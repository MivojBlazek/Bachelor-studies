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
    arr = np.power((a - b), 2)
    arr = arr.sum(axis=1)
    arr = np.sqrt(arr)
    return arr


def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    pass


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    pass


def download_data() -> Dict[str, List[Any]]:
    pass


if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
