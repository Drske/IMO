# Wymagania

Do kompilacji programu, wymagane są następujące dodatkowe biblioteki:

* **JSON for Modern C++**: [nlohman/json](https:github.com/nlohmann/json)
  * **MacOS (brew):** `brew install nlohmann-json`

# Kompilacja

Aby skompilować projekt wystarczy uruchomić proste polecenie `make`

# Uruchomienie

## Przetwaranie

Przetwarzanie pojedyczej instancji wymaga uruchomienia programu wraz z kilkoma przełącznikami:

* `-solver <nazwa>` - nazwa algorytmu rozwiązującego problem, do wyboru:
  * `nearest-neighbour`
  * `greedy-cycle`
  * `regrets`
* `-in <ścieżka>` - ścieżka do pliku z instancją
* `-out <ścieżka>` - ścieżka do pliku wynikowego
* `-start-vertex <numer>` - opcojalny argument, numer wierzchołka startowego

Przykładowe uruchomienie:

`./main -in ./data/kroA100.tsp -out ./results/res.json -solver regrets -start-vertex 10`

## Wizualizacja

Skrypt `wisualization.py` służy do wizualizacji pojedycznych wyników. Przyjmuje ścieżkę do pliku z instancją (`csv`) jako pierwszy argument i ścieżkę do pliku z wynikiem (`.json`) jako drugi argument.

Skrypt rysuje wykres instancji z dwoma zaznaczonymi cyklami.

## Testowanie

Do projektu dołączono również trzy skrypty napisane w języku python których zadaniem jest:
* `/testing/run_instances.py` - uruchomienie testów i zapisanie wyników w folderze `results`
* `/testing/gather_results.py` - stworzenie pliku `.csv` który podsumowuje zebrane wyniki
* `/testing/draw_plots.py` - dla każdego testu wizualizuje wyniki i zapisuje je w folderze `plots`

# Podział zadań

* Pipeline:
  * Wczytywanie pliku wejściowego i utworzenie macierzy odległości - Michał F.
  * Eksperymenty obliczeniowe - Michał F.
  * Wizualizacja - Michał D.
* Algorytmy:
  * zachłanny (*nearest neighbour*) + pseudokod - Michał D.
  * zachłany (*greedy cycle*) + pseudokod - Michał D.
  * z żalem (*2-regret*) + pseudokod - Michał F.