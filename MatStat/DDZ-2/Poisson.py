import numpy as np
import matplotlib.pyplot as plt
from scipy.special import factorial
import random
from statsmodels.distributions.empirical_distribution import ECDF
from scipy.stats import poisson
import scipy.stats as st
import math
import sys
# def possion(theta):
#   el,n,u = np.exp(-theta), 0, np.random.uniform(0, 1)
#   pp,fact,pow = el,1,1
#   while u > pp:
#     n += 1
#     pow *= theta
#     fact *= n
#     pp += pow*el/fact
#   return n

def emperical(viborka,chisla):
    x=[]
    for i in range(len(chisla)):
      sum = 0
      for j in range(len(viborka)):
        if viborka[j] < chisla[i]:
          sum += 1
      x.append(sum/len(viborka))
    return x

# def ecdf(sample):
#     ecdf = ECDF(sample)
#     plt.plot(ecdf.x, ecdf.y)
#     plt.show() 
# #2.1
theta = 21.5
# for i in [ 5,10,100,200,400,600,800,1000]:
#   sample = [possion(theta) for _ in range(i)]
#   print("\nВыборка из ", i, " элементов: \n", sample)
#   ecdf(sample)



#   #2.2
# theta = 21.5
# for i in [ 5,10,100,200,400,600,800,1000]:
#   sample = [possion(theta) for _ in range(i)]
#   ecdf(sample)

    
def emperical(viborka,chisla):
    x=[]
    for i in range(len(chisla)):
      sum = 0
      for j in range(len(viborka)):
        if viborka[j] < chisla[i]:
          sum += 1
      x.append(sum/len(viborka))
    return x


def DMN(t,poism,poisn):
  xm = emperical(poism,t)
  xn = emperical(poisn,t)
  supr = 0
  for i in range(len(t)):
    if abs(xn[i]-xm[i]) > supr:
      supr = abs(xn[i]-xm[i])
  return math.sqrt(len(poisn)*len(poism)/(len(poism)+len(poisn)))*supr

  
t = np.arange(0. , 50. , 0.001)
sample = np.random.poisson(theta, 5)
plt.scatter(t, emperical(sample, t), s=0.4)
plt.show()



  
  

    

# #2.2
# def emperical(t,e): 
#   x = []
#   for i in range(len(t)):
#     sum=0
#     for j in range(len(t)):
#       if t[j] <= t[i]:
#         sum += 1
#     x.append(sum/len(t))
#   return x

# def DMN(t,poism,poisn):
#   xm = emperical(t,poism)
#   xn = emperical(t,poisn)
#   supr = 0
#   for i in range(len(t)):
#     if abs(xn[i]-xm[i]) > supr:
#       supr = abs(xn[i]-xm[i])
#   return math.sqrt(len(poisn)*len(poism)/(len(poisn)+len(poism)))*supr

# def DMN_graph(t,poism,poisn):
#   xm = emperical(t,poism)
#   xn = emperical(t,poisn)
#   plt.plot(t,xm)
#   plt.plot(t,xn)
#   plt.show()


      







# #3.1 Метод моментов

# #general formula for the nth sample moment   
# def sample_moment(sample, n):       
#   summed = np.sum([el**n for el in sample])       
#   length = len(sample)       
#   return 1/length * summed      


# # 3.1 Метод макс правдоподобия
# def poisson_MLE(sample):
#   return np.sum(sample)/len(sample)

# # 3.1 Метод моментов
# def poisson_MOM(sample):
#   m1 = sample_moment(sample, 1)
#   m2 = sample_moment(sample, 2)
#   return m1**2/(m2 - m1**2)
# print("Метод моментов")
# for i in [5,10,100,200,400,600,800,100]:
#   print("n = ", i, ":", poisson_MOM(r[:i]))
  



# probabilityfunctioncomparison = lambda r: plt.hist(r, bins=np.linspace(0, 35, 36), alpha=0.5, label='counting process', ec='black', align='left')
# theta = 21.5
# for i in [1000]:
#   r = np.random.poisson(theta, i)
#   probabilityfunctioncomparison(r)
#   plt.plot(poisson.pmf(np.linspace(0, 35, 36),theta)*i)
#   plt.legend()
#   plt.show()
    
# def polygon_of_frequency(r):
#   x = np.sort(r)
#   y = np.arange(len(x))/float(len(x))
#   plt.plot(x, y)
#   plt.show()
# print("Полигон частот")
# for i in [5,10,100,200,400,600,800,1000]:
#   r = np.random.poisson(21.5, i)
#   polygon_of_frequency(r)


  