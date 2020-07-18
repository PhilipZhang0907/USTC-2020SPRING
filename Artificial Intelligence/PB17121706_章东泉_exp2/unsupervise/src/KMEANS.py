import random
import numpy as np

def cmp_matrix(list1, list2):
    if(len(list1) != len(list2)):
        return 1
    else:
        for i in range(len(list1)):
            for j in range(len(list1[i])):
                if(list1[i][j] != list2[i][j]):
                    return 1
    return 0

def KMEANS(dataSet, k):
    # 从数据样本的最大最小值之间随机选取初始质心
    maxData = []
    minData = []
    for character in dataSet:
        maxData.append(max(character))
        minData.append(min(character))
    massCenter = []
    for i in range(k):
        center = []
        for j in range(len(maxData)):
            r = random.random()
            coordinate = minData[j]*r + maxData[j]*(1-r)
            center.append(coordinate)
        massCenter.append(center)
    newSet = dataSet.transpose()
    LabelSet = []
    newCenter = []
    for i in range(len(newSet)):
        LabelSet.append(-1)
    LabelSet = [LabelSet]
    # K Means
    while(cmp_matrix(massCenter, newCenter)):
        if(len(newCenter) != 0):
            massCenter = newCenter[:]
        # 计算所有数据的标签
        for j in range(len(newSet)):
            classNum = -1
            minDis = -1
            for i in range(len(massCenter)):
                newDis = np.linalg.norm(newSet[j] - massCenter[i])
                if(newDis < minDis or minDis == -1):
                    classNum = i
                    minDis = newDis
            LabelSet[0][j] = classNum
        # 计算新的质心
        newCenter = massCenter[:]
        for i in range(k):
            coorSum = []
            labelNum = 0
            for j in range(len(LabelSet[0])):
                if(LabelSet[0][j] == i):
                    if(len(coorSum) == 0):
                        coorSum = newSet[j][:]
                    else:
                        coorSum = coorSum + newSet[j]
                    labelNum = labelNum + 1
            for p in range(len(coorSum)):
                coorSum[p] = coorSum[p]/labelNum
            if(len(coorSum) != 0):
                newCenter[i] = coorSum[:]
    dataSet = np.append(dataSet, LabelSet, axis = 0)
    resultSet = dataSet.transpose()
    # 计算轮廓系数
    Silhouette = []
    for data in resultSet:
        tmp_data = data[:]
        tmp_data = np.delete(tmp_data, -1, axis = 0)
        # calculate a
        a_sum = 0
        a_num = 0
        for others in resultSet:
            if(data[-1] == others[-1]): # having the same label
                a_sum = a_sum + np.linalg.norm(data - others)
                a_num = a_num + 1
        a_avg = a_sum / a_num
        # calculate b
        centerDis = []
        b_sum = 0
        b_num = 0
        for centers in massCenter:
            Dis = np.linalg.norm(tmp_data - centers)
            centerDis.append(Dis)
        sortDis = sorted(centerDis)
        for i in range(len(sortDis)):
            if(i == 0):
                continue
            else:
                classNo = centerDis.index(sortDis[i])
                for others in resultSet:
                    if(others[-1] == classNo):
                        b_sum = b_sum + np.linalg.norm(data - others)
                        b_num = b_num + 1
                if(b_num != 0):
                    break
        b_avg = b_sum / b_num
        data_silhouette = (b_avg - a_avg)/(max(b_avg, a_avg))
        Silhouette.append(data_silhouette)
    sil_avg = np.mean(Silhouette)
    # 返回的resultSet是输入的数据加上聚类标签
    return (resultSet, sil_avg)
