class SymbolTable():
    def __init__(self):
        #empty dictonaries
        self._global = dict()
        self._local = dict()

        #class counters indexes
        self._global_static = 0
        self._global_field = 0
        self._local_arg = 0
        self._local_var = 0
    
    #reset the local counters and dict
    def startSubroutine(self):
        self._local.clear()
        self._local_arg = 0
        self._local_var = 0

    #determine identifier and increment counter 
    def define(self, name, jType, identifier):
        identifier = identifier.upper()
        if identifier == "STATIC":
            self._global[name] = (jType, identifier, self._global_static)
            self._global_static += 1
        elif identifier == "FIELD":
            self._global[name] = (jType, identifier, self._global_field)
            self._global_field += 1
        elif identifier == "ARG":
            self._local[name] = (jType, identifier, self._local_arg)
            self._local_arg += 1
        elif identifier == "LOCAL": #aka Var
            self._local[name] = (jType, identifier, self._local_var)
            self._local_var += 1

    #count the amount of items in our dictionaries
    def varCount(self):
        identifier = identifier.upper()
        result = 0
        for info in self._local.items():
            if info[1] == identifier:
                result += 1
        for info in self._global.items():
            if info[1] == identifier:
                result += 1
        return result

    #find a specific value for a name key
    def typeOf(self, name):
        if name in self._local:
            return self._local[name][0]
        if name in self._global:
            return self._global[name][0]
    def kindOf(self, name):
        if name in self._local:
            return self._local[name][1]
        if name in self._global:
            return self._global[name][1]
        return "NONE"
    def indexOf(self, name):
        if name in self._local:
            return self._local[name][2]
        if name in self._global:
            return self._global[name][2]
    