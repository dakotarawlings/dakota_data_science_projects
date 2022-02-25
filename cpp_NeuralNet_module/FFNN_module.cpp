// neural net from scratch
#include "FFNN_headers.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>
#include <pybind11/iostream.h>
#include <pybind11/eval.h>

using namespace std;
namespace py = pybind11;

struct Connection
{
	double weight;
	double deltaWeight;
};

class SingleNeuron;

typedef vector<SingleNeuron> Layer;

//**class Neuron**

class SingleNeuron
{
public:
	SingleNeuron(unsigned numOutputs, unsigned myIndex, string);
	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) const { return m_outputVal; }
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);


//private:
	static double eta; // [0.0, 1.0] overal net training rate
	static double alpha; // [ 0.0, n] multiplyer of last wieght change (momentum)
	double activationFunction(double x);
	double activationFunctionDerivative(double x);
	static double randomWeight(void) { return rand() / double(RAND_MAX+1); }

	double sumDOW(const Layer &nextLayer) const;

	double m_outputVal;
	vector<Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;

	string functionType;
};

double SingleNeuron::eta = 0.15; // overall net learning rate 0.15
double SingleNeuron::alpha = 0.5; // momentum multiplyer of last deltaWeight [0.0, n] 0.5

void SingleNeuron::updateInputWeights(Layer &prevLayer)
{
	//The weights to be updated are in the connection container
	// in the neurons in the preceding layer
	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		// individual input magnified by the gradient and train rate
		//also add momentum= a fraction of the previous delta weight
		SingleNeuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

		double newDeltaWeight = eta * neuron.getOutputVal() * m_gradient + alpha * oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}

double SingleNeuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;
	//sum our contributions of the errors at the nodes we feed
	for (unsigned n=0; n < nextLayer.size() - 1; ++n)
	{
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}
	return sum;
}

void SingleNeuron::calcHiddenGradients(const Layer &nextLayer)
{

	double dow = sumDOW(nextLayer);
	m_gradient = dow * SingleNeuron::activationFunctionDerivative(m_outputVal);
}

void SingleNeuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * SingleNeuron::activationFunctionDerivative(m_outputVal);
}



double SingleNeuron::activationFunction(double x)
{
	double output;

	string tanhstring = "tanh";
	string relustring = "relu";
	string sigmoidstring = "sigmoid";

	if (functionType==tanhstring)
	{
		output = tanh(x);
	}

	if (functionType==relustring)
	{
		if (x >= 0)
		{
			output = x;
		}
		if (x < 0)
		{
			output = 0;
		}
	}

	if (functionType==sigmoidstring)
	{
		output = sigmoid(x);
	}
	//tanhh- output range [-1, 1]
	return output;
}

double SingleNeuron::activationFunctionDerivative(double x)
{
	//tanh derivative

	double output;
	string tanhstring = "tanh";
	string relustring = "relu";
	string sigmoidstring = "sigmoid";

	if (functionType==tanhstring)
	{
		output = 1.0 - x * x;
	}

	if (functionType==relustring)
	{
		if (x >= 0)
		{
			output = 1;
		}
		if (x < 0)
		{
			output = 0;
		}
	}

	if (functionType==sigmoidstring)
	{
		output = Dsigmoid(x);
	}

	return output;
}

void SingleNeuron::feedForward(const Layer& prevLayer)
{
	double sum = 0.0;

	//sum the previous layers output (which are inputs) and include the bias node from the previous layer
	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		sum += prevLayer[n].getOutputVal() *
			prevLayer[n].m_outputWeights[m_myIndex].weight;
	}
	m_outputVal = SingleNeuron::activationFunction(sum);
}

SingleNeuron::SingleNeuron(unsigned numOutputs, unsigned myIndex, string functionType_)
{
	functionType = functionType_;
	for (unsigned c = 0; c < numOutputs; ++c)
	{
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}
	m_myIndex = myIndex;
}

//**class net**

class NeuralNet
{
	public:
		//Net(const vector<unsigned>& topology);
		NeuralNet() {}
		void BuildNet(string);
		void feedForward(const vector<double>& inputVals);
		void backProp(const vector<double>& targetVals);
		void getResults(vector<double>& resultVals) const;
		double getRecentAverageError(void) const { return m_recentAverageError; }

		void setNetTopology(vector<double>);
	//private:
		vector<Layer> m_layers; // m_layers[layerNum][neuronNum]
		double m_error;
		double m_recentAverageError;
		double m_recentAverageSmoothingFactor;

		vector<double> topology;

};

void NeuralNet::setNetTopology(vector<double> inputTopology) {

	topology = inputTopology;

}


void NeuralNet::getResults(vector<double>& resultVals) const
{
	resultVals.clear();
	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
	{
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

void NeuralNet::backProp(const vector<double>& targetVals)
{
	// Calculate overall net error (RMS of output neuron errors)
	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; n++)
	{
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1;
	m_error = sqrt(m_error);
	// Implementat a recent average measurement
	m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);
	//Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size() - 1; n++)
	{
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}
	//claculate gradients on hidden layers
	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer& hiddenLayer = m_layers[layerNum];
		Layer& nextLayer = m_layers[layerNum + 1];
		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
}
//for all layers from outputs to fist hidden layers
//update connction wiehgts

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
	{
		Layer& layer = m_layers[layerNum];
		Layer& prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < layer.size() - 1; ++n)
		{
			layer[n].updateInputWeights(prevLayer);
		}
}


{

}
}

void NeuralNet::feedForward(const vector<double>  &inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1);
	

	//Assign (latch) the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); i++)
	{
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
	{
		Layer& prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n)
		{
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}


void NeuralNet::BuildNet(string functionType)
{	
	unsigned numLayers = topology.size();

	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
		{
			m_layers.back().push_back(SingleNeuron(numOutputs, neuronNum, functionType));
			//cout << "Made a Neuron!" << endl;
		}
		//force the bias nodes output value to 1.0 its the last neuron created above
		m_layers.back().back().setOutputVal(1.0);//1.0

	}
}



class FFNN_Builder {
	public:
		FFNN_Builder() {}
		NeuralNet NeuralNetModel;
		vector<double> modelTopology;

		vector<vector<double>> x_data;
		vector<vector<double>> y_data;
		int epochs;

		void setTopology(vector<double>, string);


		void fitModel(vector<vector<double>>, vector<vector<double>>, int);


		vector<double> predict(vector<double>);

		vector<double> inputVals, targetVals, resultVals;
		string activationType;

		string getActivationType() { return activationType; }
		vector<vector<double>> getXData() { return x_data; }
		vector<vector<double>> getYData() { return y_data; }
		vector<double> getTopology() { return modelTopology; }
		int getEpochs() { return epochs; }

};


void FFNN_Builder::setTopology(vector<double> TopologyInput, string activationType_) {
	activationType = activationType_;

	modelTopology = TopologyInput;

	NeuralNetModel.setNetTopology(modelTopology);
	NeuralNetModel.BuildNet(activationType);
}


void FFNN_Builder::fitModel(vector<vector<double>> x_data_, vector<vector<double>> y_data_, int epochs_=1) {
	
	epochs = epochs_;
	int epoch = 0;
	x_data = x_data_;
	y_data = y_data_;

	//int trainingPass = 0;
	vector<double> x_row;
	vector<double> y_row;


	while (epoch < epochs)
	{

		int trainingPass = 0;

		while (trainingPass != x_data.size() - 1)
		{

			//py::print("training pass: ", trainingPass);

			x_row = x_data[trainingPass];
			y_row = y_data[trainingPass];

			//x_row = getRow(x_data, trainingPass);
			//y_row = getRow(y_data, trainingPass);

			if (x_row.size() != modelTopology[0])
			{
				break;
			}

			//py::print("Input vals: ", x_row);

			NeuralNetModel.feedForward(x_row);

			NeuralNetModel.getResults(resultVals);

			//py::print("Output: ", resultVals);

			//py::print("Targets:", y_row);

			NeuralNetModel.backProp(y_row);

			//py::print("net recent average error:", NeuralNetModel.getRecentAverageError());

			++trainingPass;

		}
		++epoch;

	}


	py::print("Done");

}

vector<double> FFNN_Builder::predict(vector<double> x_test_row) {

	vector<double> y_predicted;

	NeuralNetModel.feedForward(x_test_row);
	NeuralNetModel.getResults(y_predicted);

	return y_predicted;

}

//maybe try adding getNet and a setNet member functions to the FFNN_builder
//need to add set and get functions to FFNN builder (probably wont just work with setting entire object, has to be set with basic types)


PYBIND11_MODULE(FFNN_pymodule, m) {
	py::class_<FFNN_Builder>(m, "FFNN_Builder")
		.def(py::init<>())
		.def("setTopology", &FFNN_Builder::setTopology)
		.def("setTopology", &FFNN_Builder::getTopology)
		.def("setTopology", &FFNN_Builder::getXData)
		.def("setTopology", &FFNN_Builder::getYData)
		.def("fitModel", &FFNN_Builder::fitModel)
		.def("predict", &FFNN_Builder::predict)
		
		.def(py::pickle(
			[](FFNN_Builder& FFNN) { // __getstate__
				/* Return a tuple that fully encodes the state of the object */
				return py::make_tuple(FFNN.getTopology(), FFNN.getXData(), FFNN.getYData(), FFNN.getActivationType(), FFNN.getEpochs());
			},
			[](py::tuple t) { // __setstate__
				if (t.size() != 5)
					throw std::runtime_error("Invalid state!");

				/* Create a new C++ instance */
				FFNN_Builder FFNN;

				/* Assign any additional state */
				FFNN.setTopology(t[0].cast<vector<double>>(), t[3].cast<string>());
				FFNN.fitModel(t[1].cast<vector<vector<double>>>(), t[2].cast<vector<vector<double>>>(), t[4].cast<int>());

				return FFNN;
			}
			));
		

}




