def Naive_Bayes(trainSet, trainLabel, testSet, encode):
    # 计算P(c)
    positive = 0
    negative = 0
    for i in range(len(trainLabel)):
        if(trainLabel[i] == 1):
            positive = positive + 1
        else:
            negative = negative + 1
    P_positive = positive / len(trainLabel)
    P_negative = negative / len(trainLabel)
    # 计算P(x|c)
    dataType = trainSet[0]
    P_x_c = []
    for i in range(len(dataType)):
        probability = []
        for data in encode[i]:
            tmp_positive = 0
            tmp_negative = 0
            for j in range(len(trainSet)):
                if(trainSet[j][i] == data and trainLabel[j] == 1):
                    tmp_positive = tmp_positive + 1
                if(trainSet[j][i] == data and trainLabel[j] == -1):
                    tmp_negative = tmp_negative + 1
            probability.append((tmp_positive / positive, tmp_negative / negative))
        P_x_c.append(probability)
    # 开始预测
    predict_label = []
    for data in testSet:
        predict_positive = P_positive
        predict_negative = P_negative
        for i in range(len(data)):
            predict_positive = predict_positive * P_x_c[i][encode[i].index(data[i])][0]
            predict_negative = predict_negative * P_x_c[i][encode[i].index(data[i])][1]
        if(predict_positive >= predict_negative):
            predict_label.append(1)
        else:
            predict_label.append(-1)
    return predict_label