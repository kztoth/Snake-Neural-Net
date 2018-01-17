main:
	g++ -Wall main.cpp NeuralNet.cpp Neuron.cpp GameState.cpp Renderer.cpp Snake.cpp DNA.cpp -o SNN -lcomdlg32 -luser32 -ladvapi32 -lgdi32