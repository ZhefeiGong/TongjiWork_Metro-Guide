# @author : gonzalez
# @time   : 2022.8.29
# @func   : 根据现有数据转化上海地铁站信息 以便项目读取

import sys
import re




if __name__ == '__main__':
    file1 = open('subwayContent.txt')
    file2 = open('subwayLongitudeLatitude_1.txt')
    lines1 = file1.readlines()
    lines2 = file2.readlines()

    # 提取原始数据
    subwayLines = []
    for each in lines1:
        each = each.strip('\n')
        eachGet = each.split(" ")
        subwayLines.append(eachGet)

    # 站点的经纬度
    location = []
    for each in lines2:
        each = each.strip('\n')
        eachGet = each.split("\t")
        eachGet = eachGet[1:]  # 删除多余首元素
        location.append(eachGet)

    # 为每个数据赋横纵坐标
    for line in subwayLines:
        for i in range(len(line)):
            if i != 0:
                for site in location:
                    if line[i] in site[0]:
                        line[i] = line[i] + ' ' + site[1] + ' '+site[2]

    # 结果数据写入subwayInfo.txt中
    fileOut = open("subwayInfo.txt", "w")
    for line in subwayLines:

        # 输出前缀
        indexValue = subwayLines.index(line)
        idLine = 'id' + ' ' + str(indexValue+1) + '\n'
        nameLine = 'name' + ' ' + line[0] + '\n'
        numLine = 'numSite' + ' ' + str(len(line)-1) + '\n'
        fromLine = 'fromTo' + ' ' + line[1].split(' ')[0] + ' ' + line[len(line)-1].split(' ')[0] + '\n'
        fileOut.write(idLine)
        fileOut.write(nameLine)
        fileOut.write(numLine)
        fileOut.write(fromLine)

        # 输出站点
        for each in (line[1:]):
            siteLine = each + '\n'
            fileOut.write(siteLine)

        # 输出终止符
        if subwayLines.index(line) == len(subwayLines)-1:
            fileOut.write('==========\n')  # 行终止符
        else:
            fileOut.write('----------\n')  # 文件终止符

    # 某些地铁站信息不完整 仍需手动获取经纬度并修改















