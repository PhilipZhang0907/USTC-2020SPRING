import math
import PCA
import KMEANS
import pandas as pd
import numpy as np
from matplotlib import pyplot as plt


def View(dataSet, K, Label):
    style = ['ro', 'bo', 'go', 'mo', 'yo', 'ko', 'wo', 'co']
    COV = np.cov(dataSet)
    eigenvalue, eigenvector = np.linalg.eig(COV)
    eigen_list = []
    for i in range(len(eigenvalue)):
        eigen_data = {'value': eigenvalue[i], 'vector': eigenvector[i]}
        eigen_list.append(eigen_data)
    sorted(eigen_list, key= lambda x:x['value'])
    matrix_P = []
    for i in range(2):
        matrix_P.append(eigen_list[i]['vector'])
    reduced_matrix = np.matmul(matrix_P, dataSet)
    plt.xlabel('x1')
    plt.ylabel('x2')
    for i in range(len(reduced_matrix.T)):
        plt.plot(reduced_matrix.T[i][0], reduced_matrix.T[i][1], style[int(Label[i])])
    plt.show()

def Write_CSV(data):
    filepath = '../output/wine.csv'
    write_file = pd.DataFrame(data)
    write_file.to_csv(filepath, index = False, sep=',')


if __name__ == '__main__':
    filename = '../input/wine.data'
    readbuffer = open(filename, 'r')
    dataSet = []
    try:
        # read data
        dataLines = readbuffer.readlines()
        for line in dataLines:
            dataBuffer = line.split(',')
            dataBuffer[-1] = dataBuffer[-1].replace('\n', '')
            for i in range(len(dataBuffer)):
                dataBuffer[i] = float(dataBuffer[i])
            dataSet.append(dataBuffer)
        # preproccessing
        # calculating AVG and SD for each characteristic
        dataSample = dataSet[0][:]
        for i in range(len(dataSample)):
            # drop first characteristic "wine type"
            if(i == 0):
                continue
            AVG = 0
            SD = 0
            # calculate AVG
            for data in dataSet:
                AVG = AVG + data[i]
            AVG = AVG / len(dataSet)
            # calculate SD
            for data in dataSet:
                SD = SD + math.pow((data[i] - AVG), 2.0)
            SD = SD / len(dataSet)
            SD = math.pow(SD, 0.5)
            for j in range(len(dataSet)):
                dataSet[j][i] = (dataSet[j][i] - AVG) / SD
        # do transposition
        dataSample = dataSet[0][:]
        data_for_PCA = []
        for i in range(len(dataSample)):
            component = []
            for data in dataSet:
                component.append(data[i])
            data_for_PCA.append(component)
        # do PCA
        threshold = 0.6
        True_Label = data_for_PCA[0][:]
        data_for_PCA.pop(0)
        reduced_data = PCA.Principal_Component_Analysis(data_for_PCA, threshold)
        # do KMEANS
        k = 3
        result_data, sil = KMEANS.KMEANS(reduced_data, k)
        Write_CSV(result_data)
        View(data_for_PCA, k, result_data[:,-1])
        # 计算兰德系数
        Predict_Label = result_data[:,-1]
        for i in range(len(True_Label)):
            True_Label[i] = int(True_Label[i])
            Predict_Label[i] = int(Predict_Label[i])
        RI_a = 0
        RI_b = 0
        RI_c = 0
        RI_d = 0
        for i in range(len(True_Label)):
            for j in range(len(True_Label)):
                if(i == j):
                    continue
                else:
                    if(True_Label[i] == True_Label[j] and Predict_Label[i] == Predict_Label[j]):
                        RI_a = RI_a + 1
                    if(True_Label[i] == True_Label[j] and Predict_Label[i] != Predict_Label[j]):
                        RI_b = RI_b + 1
                    if(True_Label[i] != True_Label[j] and Predict_Label[i] == Predict_Label[j]):
                        RI_c = RI_c + 1
                    if(True_Label[i] != True_Label[j] and Predict_Label[i] != Predict_Label[j]):
                        RI_d = RI_d + 1
        RI = (RI_a + RI_d) / (RI_a + RI_b+RI_c + RI_d)
        print(RI)
        print(sil)
    finally:
        readbuffer.close()
