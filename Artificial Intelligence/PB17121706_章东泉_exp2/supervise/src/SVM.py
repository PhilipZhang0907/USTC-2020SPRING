import numpy as np
import random

def KernelTrans(x1, x2, kernel):
    if(kernel == 'linear'):
        return np.dot(x1, x2.T)
    if(kernel == 'polynomial'):
        return (np.dot(x1, x2.T) ** 3)

def get_random(a, b, z):
    i = z
    while(i == z):
        i = random.randint(a, b)
    return i

def get_L_H(C, alpha_prime_j, alpha_prime_i, y_j, y_i):
    if(y_i != y_j):
        return (max(0, alpha_prime_j - alpha_prime_i), min(C, C - alpha_prime_i + alpha_prime_j))
    else:
        return (max(0, alpha_prime_i + alpha_prime_j - C), min(C, alpha_prime_i + alpha_prime_j))

def cal_w(alpha, trainSet, trainLabel):
    return np.dot(alpha * trainLabel, trainSet)

def cal_b(trainSet, trainLabel, w):
    b = trainLabel - np.dot(w.T, trainSet.T)
    return np.mean(b)

def h(X, w, b):
    return np.sign(np.dot(w.T, X.T) + b).astype(int)

def E(x_k, y_k, w, b):
    return h(x_k, w, b) - y_k

def SVM_Train(trainSet, trainLabel, C, kernel, max_iter, epsilon):
    n,d = np.shape(trainSet)
    alpha = np.zeros((n))
    count = 0
    w = cal_w(alpha, trainSet, trainLabel)
    b = cal_b(trainSet, trainLabel, w)
    while True:
        percent = count / max_iter
        count = count + 1
        alpha_old = np.copy(alpha)
        for j in range(0, n):
            # 生成两个不同的随机数i, j
            # 取对应的训练数据和标签
            i = get_random(0, n-1, j)
            x_i, x_j = trainSet[i][:], trainSet[j][:]
            y_i, y_j = trainLabel[i], trainLabel[j]
            # 计算核
            K_ij = KernelTrans(x_i, x_i, kernel) + KernelTrans(x_j, x_j, kernel) - 2 * KernelTrans(x_i, x_j, kernel)
            if(K_ij == 0):
                continue
            alpha_prime_i, alpha_prime_j = alpha[i], alpha[j]
            (L, H) = get_L_H(C, alpha_prime_j, alpha_prime_i, y_j, y_i)
            # 计算模型参数
            w = cal_w(alpha, trainSet, trainLabel)
            b = cal_b(trainSet, trainLabel, w)
            E_i = E(x_i, y_i, w, b)
            E_j = E(x_j, y_j, w, b)
            # 新的alpha
            alpha[j] = alpha_prime_j + float(y_j * (E_i - E_j)) / K_ij
            alpha[j] = max(alpha[j], L)
            alpha[j] = min(alpha[j], H)
            alpha[i] = alpha_prime_i + y_i * y_j * (alpha_prime_j - alpha[j])
        # 检查协方差
        diff = np.linalg.norm(alpha - alpha_old)
        if(diff < epsilon):
            break
        if(count > max_iter):
            break
    b = cal_b(trainSet, trainLabel, w)
    if(kernel == 'linear'):
        w = cal_w(alpha, trainSet, trainLabel)
    # 计算支持向量
    alpha_index = np.where(alpha > 0)[0]
    support_vectors = trainSet[alpha_index, :]
    return (support_vectors, count), (w, b)

def SVM_Predict(w, b, testSet):
    label = []
    test = np.array(testSet)
    for data in test:
        label.append(h(data, w, b))
    return label

def Support_Vector_Machine(trainSet, trainLabel, testSet, C, kernel):
    max_iter = 100
    epsilon = 0.001
    X = np.array(trainSet)
    Y = np.array(trainLabel)
    (support_vectors, count), (w, b) = SVM_Train(X, Y, C, kernel, max_iter, epsilon)
    Predict_label = SVM_Predict(w, b, testSet)
    return Predict_label