#
# This file is part of the KandVST synthesizer.
#
# Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
#                      Sarosh Nasir, Stanisław Zwierzchowski,
#                      Klas Ludvigsson and Andreas Kuszli
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

import sys
from os import walk
import os
from os import path

writeDir='Source'
if not os.path.exists(writeDir):
    os.makedirs(writeDir)
__writeFile_h = open (writeDir+"/Util/Resources_files.h",'w')
__writeFile_cpp = open (writeDir+"/Util/Resources_files.cpp",'w')
__writeFile_h.write("#ifndef RESOURCES_FILES_H\n#define RESOURCES_FILES_H\n")
__writeFile_cpp.write("#include \"Resources_files.h\"\n")
rowBreak =30




def createResourceFile(fullDir,__fileList,__name,__namespace):
    __namespace += "::" + __name
    if (len(__fileList)==0):
        return
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
        print (__fileName + " : " + str(count) + " bytes")
        
        
    return


def walkDir(dir,namespace):
    files = [f for f in os.listdir(dir) if path.isfile(path.join(dir,f))]
    dirs = [f for f in os.listdir(dir) if (not f in files)]
    name= os.path.basename(dir)
    __writeFile_h.write("namespace " + name + "{\n")
    for _dir in dirs:
        ns = ""
        if (namespace !=""):
            ns = namespace + "::"
        walkDir(path.join(dir,_dir),ns+dir)
    
    name= os.path.basename(dir)
    createResourceFile(dir,files,name,namespace)
    __writeFile_h.write("}\n")


    return


walkDir(sys.argv[1],"")

__writeFile_h.write("#endif")




