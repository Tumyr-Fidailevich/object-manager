# "Object Manager"

## Подготовка среды

1. Создайте каталог для сборки проекта:

    ```bash
    mkdir build
    cd build
    ```

2. Выполните настройку проекта с помощью CMake, указав пути к библиотеке Qt, компилятору и другим необходимым переменным:

    ```bash
    cmake -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DQT_QMAKE_EXECUTABLE=${QT_QMAKE_EXECUTABLE} ..
    ```

## Сборка проекта

```bash
cmake --build .
```

## Features

- **Undo-Redo Manager:**
  - Soon.
