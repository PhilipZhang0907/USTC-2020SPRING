import os, sys, random
import KNN
import SVM
import other


def TestPerformance(TrueLabel, PredictLabel):
    TP = 0
    FP = 0
    TN = 0
    FN = 0
    for i in range(len(TrueLabel)):
        if(TrueLabel[i] == 1 and PredictLabel[i] == 1):
            TP = TP + 1
        if(TrueLabel[i] == -1 and PredictLabel[i] == 1):
            FP = FP + 1
        if(TrueLabel[i] == 1 and PredictLabel[i] == -1):
            FN = FN + 1
        if(TrueLabel[i] == -1 and PredictLabel[i] == -1):
            TN = TN + 1
    P = TP / (TP + FP)
    R = TP / (TP + FN)
    print(TP, FP, TN, FN)
    print(P, R)

if __name__ == '__main__':
    filename = '../data/por.data'
    readBuffer = open(filename, 'r')
    dataSet = []
    try:
        text_lines = readBuffer.readlines()
        for line in text_lines:
            strBuffer = line.split(';')
            # drop attribute 'school'
            strBuffer.pop(0)
            # drop '\n'
            strBuffer[-1] = strBuffer[-1].replace('\n', '')
            # drop '"'
            for i in range(len(strBuffer)):
                strBuffer[i] = strBuffer[i].replace('"', '')
            # drop header
            if(line == text_lines[0]):
                continue
            dataSet.append(strBuffer)
        # encode string into number
        dataType = dataSet[0][:]
        for i in range(len(dataType)):
            if(dataType[i].isdigit()):
                for data in dataSet:
                    data[i] = int(data[i])
        # divide training set and testing set
        # trainging set : test set = 7 : 3
        TrainSet = []
        TrainSetLen = int(len(dataSet)*0.7)
        TestSet = []
        TestSetLen = len(dataSet) - TrainSetLen
        random.shuffle(dataSet)
        for i in range(len(dataSet)):
            if(i <= TrainSetLen):
                TrainSet.append(dataSet[i])
            else:
                TestSet.append(dataSet[i])
        # 分离出方便SVM处理的数据
        TrainLabel = []
        for data in TrainSet:
            if(data[-1] >= 10):
                TrainLabel.append(1)    # 及格成绩标为正类
            else:
                TrainLabel.append(-1)   # 不及格成绩标为负类
        for i in range(len(TrainSet)):
            TrainSet[i].pop()
            # TrainSet[i].pop()
            # TrainSet[i].pop()
        TrueTestLabel = []
        for data in TestSet:
            if(data[-1] >= 10):
                TrueTestLabel.append(1)
            else:
                TrueTestLabel.append(-1)
        for i in range(len(TestSet)):
            TestSet[i].pop()
            # TestSet[i].pop()
            # TestSet[i].pop()
        '''
        # KNN
        PredictLabel = KNN.K_NearestNeighbor(TrainSet, TrainLabel, TestSet, 6)
        '''
        # for i in range(len(TrueTestLabel)):
        #     print('(%d, %d)' %(TrueTestLabel[i], PredictLabel[i]))

        # SVM
        # 预处理，将字符量转为数字
        Encode = []
        dataType = dataSet[0]
        for i in range(len(dataType)):
            LabelEncode = []
            for j in range(len(dataSet)):
                if(LabelEncode.count(dataSet[j][i]) == 0):
                    LabelEncode.append(dataSet[j][i])
            Encode.append(LabelEncode)
        # 将离散量转为Encode中的index
        for i in range(len(dataType)):
            if(type(dataType[i]) == str):
                for j in range(len(TrainSet)):
                    TrainSet[j][i] = Encode[i].index(TrainSet[j][i])
                for k in range(len(TestSet)):
                    TestSet[k][i] = Encode[i].index(TestSet[k][i])
        PredictLabel = SVM.Support_Vector_Machine(TrainSet, TrainLabel, TestSet, 200, 'linear')
        '''
        # NB
        PredictTestLabel = other.Naive_Bayes(TrainSet, TrainLabel, TestSet, Encode)
        '''
        # 性能评估
        TestPerformance(TrueTestLabel, PredictLabel)
    finally:
        readBuffer.close()