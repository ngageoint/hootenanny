import os
import json


class GbdxTaskInterface(object):
    """A GBDX task base class.
    """

    def __init__(self, work_path="/mnt/work/"):
        self._work_path = work_path
        self._string_input_ports = None
        self._string_output_ports = None
        self._runtime_info = None
        self._reason = None
        self._status = "success"

        if not os.path.exists(self._work_path):
            raise Exception("Working path must exist. {_path}.".format(_path=self._work_path))

        string_input_ports = os.path.join(self._work_path, 'input', "ports.json")
        if os.path.exists(string_input_ports):
            with open(string_input_ports, 'r') as f:
                self._string_input_ports = json.load(f)

        gbdx_runtime = os.path.join(self._work_path, "gbdx_runtime.json")
        if os.path.exists(gbdx_runtime):
            with open(gbdx_runtime, 'r') as f:
                self._runtime_info = json.load(f)

    @property
    def base_path(self):
        return self._work_path

    @property
    def input_path(self):
        return os.path.join(self.base_path, 'input')

    @property
    def output_path(self):
        return os.path.join(self.base_path, 'output')

    @property
    def status(self):
        return self._status

    @status.setter
    def status(self, status):
        self._status = status

    @property
    def reason(self):
        return self._reason

    @reason.setter
    def reason(self, reason):
        self._reason = reason

    def get_input_string_port(self, port_name, default=None):
        """
        Get input string port value
        :param port_name:
        :param default:
        :return: :rtype:
        """
        if self._string_input_ports:
            return self._string_input_ports.get(port_name, default)
        return default

    def get_runtime_info(self, runtime_attr, default=None):
        """
        Get GBDX runtime attribute info
        :param runtime_attr:
        :param default:
        :return: :rtype:
        """
        if self._runtime_info:
            return self._runtime_info.get(runtime_attr, default)
        return default

    def get_input_data_port(self, port_name, default=None):
        """
        Get the input location for a specific port
        :param port_name:
        :param default:
        :return: :rtype:
        """
        port_location = os.path.join(self.input_path, port_name)
        if os.path.isdir(port_location):
            return port_location

        return default

    def get_multiplex_input_data_port(self, port_name_prefix):
        """
        Get the input location for a specific multiplex port
        :param port_name_prefix:
        :return: :rtype:
        """
        result_list = []
        for multiplex_port in os.listdir(self.input_path):
            if multiplex_port.startswith(port_name_prefix):
                folder_path = os.path.realpath(os.path.join(self.input_path, multiplex_port))
                if os.path.isdir(folder_path):
                    result_list.append(folder_path)
        return result_list

    def get_output_data_port(self, port_name):
        """
        Get the output location for a specific port
        :param port_name:
        :return: :rtype:
        """
        return os.path.join(self.output_path, port_name)

    def set_output_string_port(self, port_name, value):
        """
        Set output string port value
        :param port_name:
        :param value:
        :return: :rtype:
        """
        if not self._string_output_ports:
            self._string_output_ports = {}

        self._string_output_ports[port_name] = value

    def invoke(self):
        """
        The do something method
        :rtype : bool
        :raise RuntimeError:
        """
        raise RuntimeError("JobRunner Baseclass invoke is not callable")

    def finalize(self, success_or_fail, message=''):
        """
        :param success_or_fail: string that is 'success' or 'fail'
        :param message:
        """
        if self._string_output_ports:
            with open(os.path.join(self.output_path, 'ports.json'), 'w') as opf:
                json.dump(self._string_output_ports, opf, indent=4)

        with open(os.path.join(self.base_path, 'status.json'), 'w') as sf:
            json.dump({'status': success_or_fail, 'reason': message}, sf, indent=4)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type:
            self.finalize('failed', str(exc_val))
        else:
            self.finalize(self.status, self.reason)
