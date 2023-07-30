import math
import random
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab


class MyRandom:
    theta = None
    is_discrete = None

    def __init__(self, theta):
        self.theta = theta

    def generate_number(self):
        pass

    def sampling(self, size):
        sample = list()
        for i in range(size):
            sample += [self.generate_number()]
        return sample

    def plot_distr_func(self):
        pass

    def plot_prob_func(self):
        pass

    def plot_freq_polygon(self, data: list, file_name):
        if self.is_discrete:
            x_list = data
            freq = []
            for x in data:
                freq += [data.count(x) / len(data)]

        else:
            data = sorted(data)
            N = self.theta*5
            delta = (data[-1] - data[0]) / N

            left = data[0]
            right = data[0] + delta
            count = 0
            x_list = []
            freq = []
            i = 0
            while i < len(data):
                if i < len(data) and left <= data[i] < right:
                    count += 1
                    i += 1
                else:
                    x_list.append((left + right) / 2)
                    freq.append(count / (len(data) * delta))
                    if i == len(data):
                        break
                    else:
                        count = 0
                        left = right
                        right += delta
        plt.plot(x_list, freq, c="green", label="Freq_polygon")
        plt.legend()
        plt.xlabel('x')
        plt.savefig(file_name, dpi=400, bbox_inches='tight')
        plt.close()

    @staticmethod
    def plot_emperical_func(sample, file_name):
        sample.sort()
        new_sample = []
        for i in sample:
            if i not in new_sample:
                new_sample.append(i)
        Y = 0
        plt.plot([new_sample[0]-0.1, new_sample[0]], [0, 0],
                 c="blue", label="Empirical function")
        for i in range(len(new_sample)):
            Y += sample.count(new_sample[i])/len(sample)
            plt.plot([new_sample[i], new_sample[i]+0.1 if i ==
                     len(new_sample)-1 else new_sample[i+1]], [Y, Y], c="blue")

        plt.legend()
        plt.xlabel('x')
        plt.savefig(file_name, dpi=400, bbox_inches='tight')
        plt.close()
        # plt.show()


class Poisson(MyRandom):
    is_discrete = True

    def generate_number(self):
        exp = math.exp(-self.theta)
        x = 0
        prod = random.random()
        while prod > exp:
            prod *= random.random()
            x += 1
        return x

    def plot_distr_func(self):  # sourcery skip: avoid-builtin-shadow
        x_list = np.linspace(0, 2.5*self.theta, 50)
        y_list = []
        for x in x_list:
            x = math.ceil(x)
            sum = 0
            for i in range(x):
                sum += ((self.theta**i) * math.exp(-self.theta)) / \
                    math.factorial(i)
            y_list += [sum]

        plt.plot([x_list[0] - 0.1, x_list[0]],
                 [0, 0], c="red", label='distr_func')
        for i in range(len(x_list)):
            Y = y_list[i]
            plt.plot([x_list[i], x_list[i] + 0.1 if i ==
                     len(x_list) - 1 else x_list[i+1]], [Y, Y], c="red")

    def plot_prob_func(self):
        x_list = range(math.floor(2.5 * self.theta))
        y_list = []
        for x in x_list:
            x = math.floor(x)
            y_list += [(self.theta**x) *
                       math.exp(-self.theta) / math.factorial(x)]
        plt.plot(x_list, y_list, c="red", label=r'$f(t)$')


def HW2():
    theta = 21.5
    poiss = Poisson(theta)
    nlist = [1000000]
    poiss_emp_val = {}
    s = []
    for n in nlist:
        with open(f'poiss_{n}_sample.txt', 'w', encoding='utf-8') as file:
            sample = poiss.sampling(n)
            file.write(str(poiss.sampling(n)))
            file.close
        poiss_emp_val[n] = sample
        # poiss.plot_distr_func()
        # poiss.plot_emperical_func(sample, f'poiss_{n}_emperical.png')
        # poiss.plot_freq_polygon(sample, f'poiss_polygon_n_{n}.png')
        # poiss.plot_prob_func()
        # poiss.plot_freq_polygon(sample, f'poiss_polygon_prob_n_{n}.png')
    with open('poiss_Dmn', 'w', encoding='utf-8') as file:
        for n in nlist:
            for m in nlist:
                max_deviation = 0
                for x in np.linspace(0, 2.5*theta, 50):
                    f_n = len(
                        [item for item in poiss_emp_val[n] if item < x]) / n
                    f_m = len(
                        [item for item in poiss_emp_val[m] if item < x]) / m
                    if abs(f_n-f_m) > max_deviation:
                        max_deviation = abs(f_n-f_m)

                file.write('n={:6d} | m={:6d} | D={:10.8f}\n'.format(
                    n, m, math.sqrt((n*m)/(n+m))*max_deviation))

    
    with open(f'possion_X_S.txt', 'w', encoding='utf-8') as file:
        for n in nlist:
            sample_avg = 0
            for x in poiss_emp_val[n]:
                sample_avg += x
            sample_avg /= n

            sample_var = 0
            for x in poiss_emp_val[n]:
                sample_var += (x-sample_avg)**2
            sample_var /= n
            
            file.write(f'{ n } & { sample_avg } & { sample_var }\\\\\n')


if __name__ == '__main__':
    # HW2()
    poiss = Poisson(21.5)
    nlist = [5, 10, 50, 100, 200, 400, 600, 800, 1000]
    samples = []
    for n in nlist:
        sample = poiss.sampling(n)
        samples.append(sample)

    print(samples)
    for sample in samples:
        fig, ax = plt.subplots()
        Y, bins, patches = ax.hist(sample, 11, alpha=0.5)
        X = []
        for i in range(len(bins)-1):
            X.append((bins[i]+bins[i+1])/2)
        plt.plot(X, Y)
        fig.tight_layout()
        plt.show()



