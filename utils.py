import os

def cleantemp(path):
    for root, dirs, files in os.walk(path):
        for currentFile in files:
            exts = ('~')
            if any(currentFile.lower().endswith(ext) for ext in exts):
                os.remove(os.path.join(root, currentFile))

cleantemp( os.getcwd() );
