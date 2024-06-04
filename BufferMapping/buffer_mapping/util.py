class Counter:
    def __init__(self, bound):
        self._bound = bound
        self._iter = 0

    def restart(self):
        self._iter = 0

    def read(self):
        return self._iter

    def update(self):
        self._iter += 1
        if self._iter == self._bound:
            self._iter = 0

class AccessPattern:
    def __init__(self, _range, stride, start):
        '''
        all three variable is a list
        '''
        self._rng = _range
        self._st = stride
        self._start = start

class AccessIter(AccessPattern):
    def __init__(self, _range, stride, start, manual_switch=0):
        '''
        It will return a addr with the length equals to start_addr,
        which is also equals to the port number
        '''
        super().__init__(_range, stride, start)
        '''
        self._iter = [0 for _ in range(len(_range))]
        addr_offset = sum([i*j for i, j in zip(self._iter, self._st)])
        self._addr = [addr_offset + start_pos for start_pos in self._start]
        self._done = 0
        '''
        self.restart()
        self._manual_switch = manual_switch

    def getaddr(self):
        return self._addr

    def restart(self):
        self._iter = [0 for _ in range(len(self._rng))]
        addr_offset = sum([i*j for i, j in zip(self._iter, self._st)])
        self._addr = [addr_offset + start_pos for start_pos in self._start]
        self._done = 0
        '''
        self._iter = [0 for _ in range(len(self._rng))]
        self._done = 0
        self._addr = sum([i*j for i, j in zip(self._iter, self._st)]) + self._start

        '''

    def update(self):
        assert self._done == 0, "Error: no more read can make according to access pattern"
        for dim in range(len(self._iter)):
            self._iter[dim] += 1
            if dim > 0:
                self._iter[dim - 1] = 0
            if dim <len(self._iter) - 1:
                if self._iter[dim] < self._rng[dim]:
                    break
                '''

                elif self._rng[dim + 1] == 0:
                    self._done = 1
                    if self._manual_switch:
                        self.restart()
                    break
                '''
            else:
                if self._iter[dim] == self._rng[dim]:
                    self._done = 1
                    if self._manual_switch:
                        self.restart()
                    break
        addr_offset = sum([i*j for i, j in zip(self._iter, self._st)])
        self._addr = [addr_offset + start_pos for start_pos in self._start]


