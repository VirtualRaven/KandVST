import sys
from os import walk
import os
from os import path

writeDir='Resources_h'
if not os.path.exists(writeDir):
    os.makedirs(writeDir)
__writeFile = open (writeDir+"/Resources_files.h",'w')
rowBreak =0




def createResourceFile(fullDir,__fileList,__name):
    if (len(__fileList)==0):
        return
    print (__name+"\n")
    
   
    for __fileName in __fileList:
        __file = open(os.path.join(fullDir,__fileName),'rb')
        data = __file.read()
        #WriteHeader
        filename,ext= os.path.splitext(__fileName)
        __writeFile.write("char " + filename+"_"+ext[1:] +"[] = {")
        count = 0
        for byte in data:
            if (count == len(data)-1):
                __writeFile.write(hex(byte))
                break
            __writeFile.write(hex(byte)+",")
            count +=1
            if (rowBreak > 0):
                if (count % rowBreak == 0):
                    __writeFile.write("\n")
        #WriteFooter
        __writeFile.write("};\n\n")
        print (__fileName + " : " + str(count) + " bytes\n")
        
        
    return


def walkDir(dir):
    files = [f for f in os.listdir(dir) if path.isfile(path.join(dir,f))]
    dirs = [f for f in os.listdir(dir) if (not f in files)]
    print (dirs)
    print (files)
    name= os.path.basename(dir)
    __writeFile.write("namespace " + name + "{\n")
    for _dir in dirs:

        print(_dir)
        walkDir(path.join(dir,_dir))
    
    name= os.path.basename(dir)
    createResourceFile(dir,files,name)
    __writeFile.write("}\n")


    return




print(sys.argv)

walkDir(sys.argv[1])



