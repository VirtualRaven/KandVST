import sys
from os import walk
import os
from os import path

writeDir='Source'
if not os.path.exists(writeDir):
    os.makedirs(writeDir)
__writeFile_h = open (writeDir+"/Resources_files.h",'w')
__writeFile_cpp = open (writeDir+"/Resources_files.cpp",'w')
__writeFile_h.write("#ifndef RESOURCES_FILES_H\n#define RESOURCES_FILES_H\n")
__writeFile_cpp.write("#include \"Resources_files.h\"\n")
rowBreak =30




def createResourceFile(fullDir,__fileList,__name,__namespace):
    __namespace += "::" + __name
    if (len(__fileList)==0):
        return
    print (__name+"\n")
    
   
    for __fileName in __fileList:
        __file = open(os.path.join(fullDir,__fileName),'rb')
        data = __file.read()
        #WriteHeader
        filename,ext= os.path.splitext(__fileName)
        __writeFile_h.write("extern unsigned char " + filename+"_"+ext[1:] +"[" + str(os.path.getsize(os.path.join(fullDir,__fileName))) +"];\n")
        
        __writeFile_cpp.write("unsigned char " + __namespace+"::"+filename+"_"+ext[1:] +"[] = {")
        count = 0
        for byte in data:
            if (count == len(data)-1):
                __writeFile_cpp.write(hex(byte))
                break
            __writeFile_cpp.write(hex(byte)+",")
            count +=1
            if (rowBreak > 0):
                if (count % rowBreak == 0):
                    __writeFile_cpp.write("\n")
        #WriteFooter
        __writeFile_cpp.write("};\n\n")
        print (__fileName + " : " + str(count) + " bytes\n")
        
        
    return


def walkDir(dir,namespace):
    files = [f for f in os.listdir(dir) if path.isfile(path.join(dir,f))]
    dirs = [f for f in os.listdir(dir) if (not f in files)]
    print (dirs)
    print (files)
    name= os.path.basename(dir)
    __writeFile_h.write("namespace " + name + "{\n")
    for _dir in dirs:

        print(_dir)
        ns = ""
        if (namespace !=""):
            ns = namespace + "::"
        walkDir(path.join(dir,_dir),ns+dir)
    
    name= os.path.basename(dir)
    createResourceFile(dir,files,name,namespace)
    __writeFile_h.write("}\n")


    return




print(sys.argv)

walkDir(sys.argv[1],"")

__writeFile_h.write("#endif")




