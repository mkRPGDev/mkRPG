#!/usr/bin/python3
# -*- coding: utf-8 -*-

import os.path as ospath
from os import scandir
from sys import path as env_path

def get_main_directory():
    return ospath.dirname(ospath.abspath(ospath.dirname(__file__)))

def complete_path(path, prefix):
    if type(path) == list:
        path.insert(0, prefix)
    else:
        path = [prefix, path]
    return path


class Archi():
    """
    This class manages the arhitecture of the project.
    It allows the user to travel in the file system of the game, to get
    the XML files and others (PNG, configuration files...)
    Moreover, it should be cross-platform compliant
    """

    def __init__(self, prefix=None):
        if prefix != None:
            self.main_directory = ospath.join(get_main_directory(), prefix)
        else:
            self.main_directory = get_main_directory()

    def _get_file(self, file_path, mode='r'):
        """
        It receives a list of succeeding directories. If the file pointed by
        the argument ```file_path``` exists, it returns the opened file in mode
        ```mode```.
        """
        joined_path = ospath.join(self.main_directory, *file_path)
        if ospath.isfile(joined_path):
            return open(joined_path, mode=mode)
        else:
            raise FileNotFoundError("File does not exist")

    def get_src_file(self, file_path, mode='r'):
        """
        Gets the path of the src directory.
        At least used by the src scripts.
        """
        file_path = complete_path(file_path, 'src')
        return self._get_file(file_path, mode)

    def get_static_file(self, file_path, mode='r'):
        """
        Gets the path of the static files directory.  Static files are
        basically all graphical files, and a description of the common world
        """
        file_path = complete_path(file_path, 'static')
        return _get_file(file_path, mode)

    def get_xml_file(self, file_path, mode='r'):
        """
        Gets the path of a xml file describing a world, a scenario, or a
        campaign.
        """
        file_path = complete_path(file_path, 'xml')
        return _get_file(file_path, mode)

    def list_files(self, dir_path):
        """
        List all the files present in the dir_path, if it is a dir.
        Else raise a FileNotFoundError.
        """
        directory = self._get_dir(dir_path)
        result = []
        for entry in scandir(directory):
            if not entry.name.startswith('.') and entry.is_file():
                result.append(entry)
        return result

    def _get_dir(self, dir_path):
        """
        This general method gets the absolute path of a dir, very useful for a
        communication with a specific directory.
        """
        joined_path = ospath.join(self.main_directory, *file_path)
        if ospath.isdir(joined_path):
            return joined_path
        else:
            raise FileNotFoundError("Directory does not exist")

    def get_src_dir(self, dir_path):
        """
        Gets the given dir_path with respect to the src folder.
        """
        dir_path = complete_path(dir_path, 'src')
        return self._get_dir(dir_path)

    def get_static_dir(self, dir_path):
        """
        Gets the given dir_path with respect to the static folder.
        """
        dir_path = complete_path(dir_path, 'static')
        return self._get_dir(dir_path)

    def get_xml_dir(self, dir_path):
        """
        Gets the given dir_path with respect to the xml folder.
        """
        dir_path = complete_path(dir_path, 'xml')
        return self._get_dir(dir_path)

