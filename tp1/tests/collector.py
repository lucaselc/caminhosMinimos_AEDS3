import subprocess
from statistics import mean
from dataclasses import dataclass
import json

@dataclass
class AvgTimes:
    sys: float
    user: float
    phys: float

    def toJSON(self):
        return (self.sys, self.user, self.phys)

class Times:
    def __init__(self, name) -> None:
        self.name = name
        self.sys = []
        self.user = []
        self.phys = []

    def push(self, sys, user, phys):
        self.sys.append(sys)
        self.user.append(user)
        self.phys.append(phys)

    def avg(self) -> AvgTimes:
        return AvgTimes(*(mean(t) for t in [self.sys, self.user, self.phys]))

    def print_avg(self):
        avg = self.avg()
        print(self.name + ':')
        print(f'Sys: {avg.sys}')
        print(f'User: {avg.user}')
        print(f'Phys: {avg.phys}')



def avgs_of(input_file: str) -> tuple[AvgTimes, AvgTimes]:
    iotimes = Times('Io times')
    runtimes = Times('Algorithm times')

    def run_program(input_file):
        result = subprocess.run(['./runner', '-i', input_file, '-o', '/dev/null'], capture_output=True)

        if result.returncode != 0:
            print("Erro! ")
            print(result.stderr.decode('utf-8'))
            raise Exception('Falha ao executar programa ' + input_file)

        lines = [float(line.strip()) for line in result.stdout.split(b'\n') if len(line.strip()) > 0]

        iotimes.push(*lines[:3])
        runtimes.push(*lines[3:])

    run_count = 5
    for _ in range(run_count):
        run_program(input_file)


    return (iotimes.avg(), runtimes.avg())
