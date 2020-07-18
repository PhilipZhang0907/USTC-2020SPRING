import numpy as np

def eigen_value_sum(eigen_list, m):
    result = 0
    for i in range(m):
        result = result + eigen_list[i]['value']
    return result

def check_threshold(eigen_list, m, threshold):
    sum_all = eigen_value_sum(eigen_list, len(eigen_list))
    sum_m = eigen_value_sum(eigen_list, m)
    sum_m_1 = eigen_value_sum(eigen_list, m+1)
    if((sum_m/sum_all < threshold) and (threshold <= sum_m_1/sum_all)):
        return True
    else:
        return False

def Principal_Component_Analysis(dataSet, threshold):
    COV = np.cov(dataSet)
    eigenvalue, eigenvector = np.linalg.eig(COV)
    eigen_list = []
    for i in range(len(eigenvalue)):
        eigen_data = {'value': eigenvalue[i], 'vector': eigenvector[i]}
        eigen_list.append(eigen_data)
    sorted(eigen_list, key= lambda x:x['value'])
    k = 0
    for m in range(len(eigen_list)):
        if(check_threshold(eigen_list, m, threshold) == True):
            k = m
            break
    print(k)
    matrix_P = []
    for i in range(k):
        matrix_P.append(eigen_list[i]['vector'])
    reduced_matrix = np.matmul(matrix_P, dataSet)
    return reduced_matrix
