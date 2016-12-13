#!/usr/bin/python3.4

# Project 3 - CIS 452
# A disk utility implementation in Python
# By: Hayden Miedema

import stat
import os, sys
import collections
import argparse
import csv

# Helper function to convert from bytes to human readable
def convert_to_human(num, suffix='B'):
    for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
        if abs(num) < 1024.0:
            return "%3.1f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f%s%s" % (num, 'Yi', suffix)

# list to hold file type extensions and count
type_count = {}

def read_filesys(path, dirInfo):
    fileCount = 0
    size_in_bytes = 0

    #add the size of file
    size_in_bytes += os.lstat(path).st_size

    # loop to iterate through each file in directory
    for filex in os.listdir(path):
        # append file name to path
        pathname = os.path.join(path, filex)

        # run lstat on the path/to/file
        stat_stats = os.lstat(pathname)

        # get mode from path/to/file
        mode = stat_stats.st_mode

        # if statement to check if path is file, directory, or sym link
        if stat.S_ISDIR(mode):
            # path is a directory, must recurse deeper
            size, num = read_filesys(pathname, dirInfo)
            fileCount += num
            size_in_bytes += size
            # create a new entry in directory info list
            dirInfo[pathname] = []
            dirInfo[pathname].append(size)
            dirInfo[pathname].append(num)
        elif stat.S_ISREG(mode):
            # path is a file, we are at bottom, no more recursion
            # add size of file and move on
            fileCount += 1
            size_in_bytes += stat_stats.st_size
            # Check if file type is in dictionary and increment count
            splitPath = filex.split('.')
            if (len(splitPath) > 1):
                type = splitPath[-1]
                if type not in type_count:
                    type_count[type] = 1
                else:
                    type_count[type] += 1
        else:
            # Unknown file type, print a message
            pass

    return (size_in_bytes, fileCount)


# main method
if __name__ == '__main__':
    #initialize argument parser
    arg_parser = argparse.ArgumentParser(description='du.py - A disk usage implementation, like du written in python', conflict_handler='resolve')

    # set up arg parser to handle 1 argument
    clarg = arg_parser.add_mutually_exclusive_group()

    # -c used for outputting count of files in each directory
    clarg.add_argument('-c', dest = 'count', action = 'store_true')

    # -h used for outputting results in human readable format
    clarg.add_argument('-h', dest = 'human', action = 'store_true')

    # -t used for outputting number of each file type in a given directory
    clarg.add_argument('-t', dest = 'type', action = 'store_true')

    # -s used for outputting the results sorted by size
    clarg.add_argument('-s', dest = 'sorted', action = 'store_true')

    # adds the path to arguments
    arg_parser.add_argument('path', nargs='?', default=os.curdir, help='Give the path to directory to run du on')
    args = arg_parser.parse_args()

    # Verify that the path is valid
    if (os.path.exists(args.path) != True):
        print('Ivalid input, try again.')
        sys.exit()

    # instantiates directory as ordered to remember order of entries
    directory = collections.OrderedDict()

    # first call of read_filesys
    size, countFiles = read_filesys(args.path, directory)

    # instantiates a clear directory, then adds size and file count
    directory[args.path] = []
    directory[args.path].append(size)
    directory[args.path].append(countFiles)
  
    # Not currently used
    # csvfile = "~/PycharmProjects/duProj/output.csv"

    # Print output based on command line arguments
    if args.sorted:
        # Print ordered output
        for key,value in sorted(directory.items(), key=lambda func: func[1][0], reverse=True):
            someList = list(directory.items())
            print('%s\t%s' % (value[0], key))

    elif args.human:
        # Print human readable output
        for key,value in directory.items():
            print('%s\t  %s' % (convert_to_human(value[0]), key))
    elif args.count:
        # Print file count
        for key,value in directory.items():
            print('%s\t%s' % (value[1], key))

    elif args.type:
        for key,value in sorted(type_count.items(), key=lambda e: e[1], reverse=True)[0:24]:
            print('{}   {}'.format(key,value))
            # not currently used
            #listWriter = csv.DictWriter(
               # open('~/PycharmProjects/duProj/output.csv', 'wb'),
               # fieldnames=directory[value[1], key],
                #delimiter=',',
                #quotechar='|',
                #quoting=csv.QUOTE_MINIMAL
            #)
            #listWriter.writerow(directory[value[1], key])
    else:
        # Print normal ouput in bytes
        for key,value in directory.items():
            print('%s\t%s' % (value[0], key))

