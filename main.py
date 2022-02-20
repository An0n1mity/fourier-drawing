# This is a sample Python script.

# Press Maj+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.




from svgpathtools import parse_path
import sys
import re

def LineToArguments(line):
    pointslist = re.findall(r"[-+]?(?:\d*\.\d+|\d+)", line)
    x1, y1, x2, y2 = pointslist
    return " L {},{} {},{}".format(x1, y1, x2, y2)

def CubicBezierToArguments(cubic_bezier):
    cubic_bezier = cubic_bezier.replace('control1', '').replace('control2', '')
    pointslist = re.findall(r"[-+]?(?:\d*\.\d+|\d+)", cubic_bezier)
    x1, y1, x2, y2, x3, y3, x4, y4 = pointslist
    return " C {},{} {},{} {},{} {},{}".format(x1, y1, x2, y2, x3, y3, x4, y4)


if __name__ == '__main__':
    pathargument = sys.argv[1]
    filepath = sys.argv[2]
    path_alt = parse_path(pathargument)

    pathlist = str(path_alt).replace('Path(', '').replace(' ', '').replace('(', ' ').replace(')', '').replace('=', ' ').replace('+', ' ').replace('j', ' ').split('\n')

    path = ""
    for commands in pathlist:
        if commands[0] == 'L':
            path = path + LineToArguments(commands)
        elif commands[0] == 'C':
            path = path + CubicBezierToArguments(commands)
    path += '\n'


    print(path)
    sys.exit()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
