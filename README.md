
# Fractal Explorer

## Description

This project is a fractal visualization tool written in C using the MLX library. It generates and displays pseudo-infinite fractals, including the Mandelbrot set, Julia set, and Burning Ship fractal. Users can explore these mesmerizing mathematical structures with interactive controls and real-time rendering.

## Features

- Visualization of Mandelbrot set
- Interactive Julia set exploration
- Burning Ship fractal rendering
- Smooth color gradients
- Zoom and pan functionality
- Real-time parameter adjustments

## Video Demos

https://github.com/user-attachments/assets/6f8a1df8-42b4-4573-b820-19fffc1c6b4b



https://github.com/user-attachments/assets/6cd92fef-6311-4dda-af96-d7db986e0923



https://github.com/user-attachments/assets/e7db057a-1761-490a-afea-123f3fcd0e16



https://github.com/user-attachments/assets/e856ee4a-3f3b-4d93-aac2-41c4c863a320


## Requirements

- C compiler (GCC recommended)
- MLX library
- X11 development libraries

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/tinsights/fractal-explorer.git
   ```

2. Navigate to the project directory:
   ```
   cd fractal-explorer
   ```

3. Compile the project:
   ```
   make
   ```

## Usage

Run the program with:

```
        ./fractol [mandelbrot | burning]
        ./fractol julia [RE(Z)] [IM(Z)]
```

### Controls

- Arrow keys: Pan the view
- Mouse wheel: Zoom in/out
- Left mouse button: Set new Julia set parameter (in Julia mode)
- 'M': Switch to Mandelbrot set
- 'J': Switch to Julia set
- 'B': Switch to Burning Ship fractal
- 'R': Reset view
- 'ESC': Exit the program

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
