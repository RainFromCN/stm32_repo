print('please input the data')

dev  = []
port = []
pin  = []

while True:
    sbuf = input()
    if sbuf == 'end':
        break
    slist = sbuf.split()
    dev.append(slist[0])
    port.append(slist[1])
    pin.append(slist[2])



for i in range(len(dev) ):
    print('GPIO_TypeDef*       ', end = '')
    print(port[i])
    print('uint16_t            ', end = '')
    print(pin[i], end = '\n\n')


for i in range(len(dev) ):
    print('switch(', end = '')
    print(dev[i], end = '')
    print('[1]) {\n', end = '')
    for j in range(9):
        print('\t', end = '')
        print("case '", end = '')
        print(chr(ord('A') + j), end = '')
        print("': ", end = '')
        print(port[i], end = '')
        print(' = (GPIO_TypeDef *)GPIO', end = '')
        print(chr(ord('A') + j), end = '')
        print('_BASE; break;\n', end = '')   
    print('}\n', end = '')

    print('if (', end = '')
    print(dev[i], end = '')
    print("[3] == \'\\0\') {\n\t", end = '')
    print('switch(', end = '')
    print(dev[i], end = '')
    print('[2]) {\n', end = '')
    for j in range(10):
        print("\t\tcase '", end = '')
        print(j, end = '')
        print("': ", end = '')
        print(pin[i], end = '')
        print(' = GPIO_PIN_', end = '')
        print(j, end = '')
        print('; break;\n', end = '')
    print('\t}\n}\n', end = '')

    print('else {\n', end = '')
    print('\tswitch (', end = '')
    print(dev[i], end = '')
    print('[3]) {\n', end = '')
    for j in range(6):
        print("\t\tcase '", end = '')
        print(j, end = '')
        print("': ", end = '')
        print(pin[i], end = '')
        print(' = GPIO_PIN_', end = '')
        print(10 + j, end = '')
        print('; break;\n', end = '')
    print('\t}\n}\n', end = '')

    print()

while True:
    x = input()
    
        
