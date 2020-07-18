def Calculate_Distance(list1, list2):
    result = 0
    for i in range(len(list1)):
        if(type(list1[i]) == int):
            result = result + abs(list1[i] - list2[i])
        else:
            if(list1[i] == list2[i]):
                result = result + 1
    return result

def K_NearestNeighbor(TrainSet, TrainLabel, TestSet, K):
    predict_label = []
    for data in TestSet:
        DistanceList = []
        for i in range(len(TrainSet)):
            distance = Calculate_Distance(data, TrainSet[i])
            CaledData = {'Data': TrainLabel[i], 'Value': distance}
            DistanceList.append(CaledData)
        DistanceList = sorted(DistanceList, key=lambda x:x['Value'])
        passNum = 0
        failNum = 0
        for i in range(K):
            if(DistanceList[i]['Data'] == 1):
                passNum = passNum + 1
            else:
                failNum = failNum + 1
        if(passNum >= failNum):
            predict_label.append(1)
        else:
            predict_label.append(-1)
    return predict_label