import json
from multiprocessing.queues import SimpleQueue
from queue import Queue

import pandas as pd
import numpy as np


def calc_pi_with_monte_carlo(num: int, queue: Queue|SimpleQueue):
    n_in_circle = 0
    for _ in range(0, num):
        x = np.random.random()
        y = np.random.random()
        if np.sqrt(x ** 2 + y ** 2) <= 1:
            n_in_circle += 1
    pi = 4 * n_in_circle / num
    queue.put(pi)


def add_result_to_json(results, filename='results.json'):
    with open(file=filename, mode='w+') as f:
        json.dump(results, f, indent=4)


def add_result_to_csv(results, filename='results.csv'):
    df = pd.DataFrame(results)
    df.to_csv(filename)
