#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: Michal Blažek <xblaze38>

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Callable, Dict, Any


def distance(a: np.array, b: np.array) -> np.array:
    """This function calculate euclidean distance for each list in arrays a and b.
    Arrays a and b should have same size.
    
    Args:
        a (np.array): Array of A values in euclidean distance.
        b (np.array): Array of B values in euclidean distance.
        
    Returns:
        np.array: Array of calculated euclidean distance.
    """
    arr = np.power((a - b), 2)
    arr = arr.sum(axis=1)
    arr = np.sqrt(arr)
    return arr


def format_func(value, tick_number):
    """This function only format pi labels on a graph to LaTeX format.
    
    Args:
        value (float): A multiple of pi value that represents a point on a graph. 
        tick_number (int): A tick number on a graph.
        
    Returns:
        str: A LaTeX formatted string representing the value with pi.
    """
    N = int(np.round(2 * value / np.pi))
    if N == 0:
        # Zero is zero
        return "0"
    elif N % 2 > 0:
        # Display LaTeX format of fractions
        return f"$\\frac{{{N}}}{{2}}\\pi$"
    elif N // 2 == 1:
        # 1pi is just pi
        return f"$\\pi$"
    else:
        # Others are multiples of pi
        return f"${N // 2}\\pi$"


def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    """Generates graph of function f(x) = a^2 * sin(x).
    
    Args:
        a (List[float]): A list of float values representing the coefficients in the function.
        show_figure (bool): Allows plt.show() to display graph. Defaults to False.
        save_path (str | None): If set, graph is saved as image to file with this name. Defaults to None.
    
    Returns:
        None:
    """
    # pi = np.pi   # If we use pi more, it is better to save into variable. Now it is faster to use np.pi
    
    # Set float matrix of values using broadcasting
    x = np.arange(0, 6*np.pi + 0.1, 0.1)
    x_value = (np.array(a) ** 2) * np.sin(x).reshape(-1, 1)
    
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot()
    
    # Graphical functions and formatting
    ax.set_xlabel('x')
    ax.set_ylabel(f'$f_{{a}}(x)$')
    ax.set_xlim(0, 6 * np.pi)
    ax.set_ylim(-55, 55)
    ax.xaxis.set_major_locator(plt.MultipleLocator(np.pi / 2))
    ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func))
    
    # Fill area between sinus and x-axis according to each row in transposed matrix (each function)
    for i, _ in enumerate(a):
        ax.fill_between(x, x_value.T[i], 0, alpha=0.1)
    ax.plot(x, x_value, label=[f'$y_{{{a_value}}}(x)$' for a_value in a])
    
    # Set legend to upper center (1.13 to put it above graph)
    ax.legend(loc='upper center', bbox_to_anchor=(0.5, 1.13), ncol=len(a))
    
    # Save must be before show because show closes figure
    if save_path is not None:
        fig.savefig(save_path)
    if show_figure:
        plt.show()


# Gaps in sorted array finder
def find_gaps(arr):
    """Find gaps in a sorted array of integers.
    
    Args:
        arr (NDarray[int]): A sorted NumPy array of integers.
    
    Yields:
        tuple[int, int]: A tuple contains first integer as last number before gap.
                         A second integer is a first number in next segment after that gap.
    """
    for i in range(len(arr) - 1):
        if arr[i] + 1 < arr[i + 1]:
            yield (arr[i], arr[i + 1])


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    """Generates 3 graphs of functions:
        f1(x) = 0.5 * cos(1/50 * pi * t),
        f2(x) = 0.25 * (sin(pi * t) + sin(3/2 * pi * t)),
        f3(x) = f1(x) + f2(x).
    
    Args:
        show_figure (bool): Allows plt.show() to display graph. Defaults to False.
        save_path (str | None): If set, graph is saved as image to file with this name. Defaults to None.
    
    Returns:
        None:
    """
    # Step must be smaller because green graph is not precise at all
    step = 0.01
    half_of_graph = int(100 / step / 2)
    t = np.arange(0, 100 + step, step)
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 13), sharex=True)
    
    # Graphical functions and formatting
    ax1.set_ylabel(f'$f_{{1}}(t)$')
    ax2.set_ylabel(f'$f_{{2}}(t)$')
    ax3.set_ylabel(f'$f_{{1}}(t) + f_{{2}}(t)$')
    
    ax3.set_xlim(0, 100)
    ax3.set_xticks(np.arange(0, 101, 25))
    for ax in (ax1, ax2, ax3):
        ax.set_ylim(-0.8, 0.8)
        ax.set_yticks(np.arange(-0.8, 0.81, 0.4))

    # Graph functions to display
    ax1_func = 0.5 * np.cos(1/50 * np.pi * t)
    ax2_func = 0.25 * (np.sin(np.pi * t) + np.sin(3/2 * np.pi * t))
    ax3_func = ax1_func + ax2_func
    condition_to_green = (ax3_func > ax1_func)
    
    # Graph setting
    ax1.plot(t, ax1_func)
    ax2.plot(t, ax2_func)
    ax3.plot(t[half_of_graph:], ax3_func[half_of_graph:], color='orange')
    ax3.plot(t[:half_of_graph], ax3_func[:half_of_graph], color='red')
    # ax3.plot(t[condition_to_green], ax3_func[condition_to_green], color='green') # Still contiguous
    
    # Create discrete points of contiguous graph with successful condition
    green_points = np.where(condition_to_green)[0]
    gaps_generator = find_gaps(green_points)
    previous_last_point = green_points[0]
    segments = []

    # Create segments with always updating previous last gap that is used as start in next iteration
    for gap in gaps_generator:
        segments.append((previous_last_point, gap[0]))
        previous_last_point = gap[1]
    # Append last segment using last green point
    segments.append((previous_last_point, green_points[-1]))

    # Colorize all segments to green
    for first, second in segments:
        ax3.plot(t[first:second + 1], ax3_func[first:second + 1], color='green')

    # Save must be before show because show closes figure
    if save_path is not None:
        fig.savefig(save_path)
    if show_figure:
        plt.show()


def download_data() -> Dict[str, List[Any]]:
    """Function downloads data from website and format them into dictionary.
    
    Args:
        None:
    
    Returns:
        Dict[str, List[Any]]: Returns a dictionary that contains 'position', 'lats', 'longs' and 'heights' keys.
                              - 'positions' List[str]:   A list of position names.
                              - 'lats'      List[float]: A list of latitude values.
                              - 'longs'     List[float]: A list of longitude values.
                              - 'heights'   List[float]: A list of height values.
    """
    # TODO incorrect data loading (should be from 'https://ehw.fit.vutbr.cz/izv')
    # Get data from website and find all table rows with class 'nezvyraznit'
    resp = requests.get('https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html')
    resp.encoding = 'utf-8'
    soup = BeautifulSoup(resp.text, 'html.parser')
    all_tr = soup.find_all(class_='nezvyraznit')
    
    data = {
        'positions': [],
        'lats': [],
        'longs': [],
        'heights': []
    }
    
    # There are table cells in table rows
    for tr in all_tr:
        all_td = tr.find_all('td')
        data_in_tds = []
        for td in all_td:
            # Data formatting
            data_text = td.get_text(strip=True) \
                          .replace('°', '') \
                          .replace(',', '.') \
                          .strip("'")
            data_in_tds.append(data_text)
        # Append data to specific list in dictionary
        data['positions'].append(data_in_tds[0])
        data['lats'].append(float(data_in_tds[2]))
        data['longs'].append(float(data_in_tds[4]))
        data['heights'].append(float(data_in_tds[6]))
    return data


if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
