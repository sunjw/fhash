#
# Package for npp
# Author: Sun Junwen
# Date: 2022-03-03
#
import logging
import os
import platform
import re

from logging.handlers import RotatingFileHandler
from subprocess import call

# pip3 install pywin32
from win32api import GetFileVersionInfo, LOWORD, HIWORD

log_path = './log/debug.log'
logger = logging.getLogger('RunTime')
logging.basicConfig(
    format = '%(asctime)s %(levelname)s [%(filename)s line:%(lineno)d] - %(message)s',
    level = logging.DEBUG,
    handlers = [
        #RotatingFileHandler(logPath, maxBytes=10000000, backupCount=10),
        logging.StreamHandler()
    ])

VERSION_FILE_H = './source/version.h'
RELEASED_FILES_DIR = './x64/Release'
EXE_FILE_NAME = 'fHash64.exe'
DLL_FILE_NAME = 'fHashShlExt64.dll'

version = 0

def is_windows_sys():
    return (platform.system() == 'Windows')

def read_file(file_path):
    file_content = ''
    with open(file_path) as f:
        file_content = f.read()
    return file_content

def read_version_h():
    version_file_content = read_file(VERSION_FILE_H)
    reg = re.compile('STR_VERSION "(.*)"')
    #logger.info(version_file_content)
    reg_match = reg.search(version_file_content)
    version_value = reg_match.group(1)
    #logger.info(version_value)
    version_parts = version_value.split('.')
    if len(version_parts) > 3:
        version_value = version_parts[0] + '.' + version_parts[1] + '.' + version_parts[2]
    return version_value

def read_version_exe(filename):
    info = GetFileVersionInfo(filename, '\\')
    ms = info['FileVersionMS']
    ls = info['FileVersionLS']
    return '%s.%s.%s.%s' % (HIWORD(ms), LOWORD(ms), HIWORD(ls), LOWORD(ls))

def package_release():
    global version
    version_full = version + '.0'

    cwd = os.getcwd()
    #logger.info(cwd)

    released_files_dir_full = os.path.join(cwd, RELEASED_FILES_DIR)

    logger.info('Package...')
    os.chdir(released_files_dir_full)
    win64_zip_full = os.path.join(released_files_dir_full, 'fHash64-%s-win64.zip' % (version))
    #logger.info(win64_zip_full)
    exe_win64_version = read_version_exe(EXE_FILE_NAME)
    #logger.info(exe_win64_version)
    if not exe_win64_version == version_full:
        logger.error('"%s" version not match, want: %s, found %s' % (EXE_FILE_NAME, version, exe_win64_version))
        return
    dll_win64_version = read_version_exe(DLL_FILE_NAME)
    #logger.info(dll_win64_version)
    if not dll_win64_version == version_full:
        logger.error('"%s" version not match, want: %s, found %s' % (DLL_FILE_NAME, version, dll_win64_version))
        return
    cmd_list = ['7z', 'a', win64_zip_full]
    cmd_list.extend([EXE_FILE_NAME, DLL_FILE_NAME])
    call(cmd_list)
    # Go back
    os.chdir(cwd)

def main():
    global version

    if not is_windows_sys():
        logger.error('Only support Windows.')
        return

    os.chdir(os.path.dirname(__file__))

    version = read_version_h()
    logger.info('Version: %s' % (version))

    package_release()


if __name__ == '__main__':
    main()
