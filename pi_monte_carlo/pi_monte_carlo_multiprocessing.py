from multiprocessing import Process, Queue
from time import perf_counter

from utils import calc_pi_with_monte_carlo, add_result_to_csv


def run_pi_calculations_multiprocessing(num_sample: int, num_processes: int) -> dict:
    processes = []
    result_queue = Queue()

    samples_per_process = num_sample // num_processes
    remaining_samples = num_sample % num_processes
    
    start = perf_counter()
    
    for i in range(num_processes):
        samples = samples_per_process + (remaining_samples if i == num_processes - 1 else 0)
        p = Process(target=calc_pi_with_monte_carlo, args=(samples, result_queue))
        processes.append(p)
        p.start()
    
    for p in processes:
        p.join()
    
    elapsed_time = perf_counter() - start
    pi_value = result_queue.get()
    
    return {
        'num_samples': num_sample,
        'pi_value': pi_value,
        'elapsed_time': elapsed_time,
        'num_processes': num_processes
    }


if __name__ == "__main__":
    sample_sizes = [10 ** i for i in range(4, 9)]
    process_counts = [1, 2, 4, 8]
    results = []
    
    for num_of_processes in process_counts:
        for num_samples in sample_sizes:
            result = run_pi_calculations_multiprocessing(num_samples, num_processes=num_of_processes)
            results.append(result)
    
    add_result_to_csv(results=results, filename='pi_calculating_in_multiprocessing.csv')
