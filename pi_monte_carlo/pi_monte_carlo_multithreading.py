from time import perf_counter
from queue import Queue
from threading import Thread

from utils import add_result_to_csv, calc_pi_with_monte_carlo


def run_pi_calculations(result_list: list, num_sample: int) -> list:
    q = Queue()
    t = Thread(target=calc_pi_with_monte_carlo, args=(num_sample, q))
    start = perf_counter()
    t.start()
    t.join()
    elapsed_time = perf_counter() - start
    pi_value = q.get()
    
    result_list.append({'num_samples': num_sample, 'pi_value': pi_value, 'elapsed_time': elapsed_time})
    return result_list


if __name__ == '__main__':
    n_samples_list = [10 ** i for i in range(4, 9)]
    result_list_ = []
    for nums in n_samples_list:
        result = run_pi_calculations(result_list=result_list_, num_sample=nums)
    add_result_to_csv(results=result_list_)
