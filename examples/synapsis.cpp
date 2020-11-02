/*************************************************************

Copyright (c) 2006, Fernando Herrero Carrón
Copyright (c) 2020, Angel Lareo <angel.lareo@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of the author nor the names of his contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*************************************************************/

#include <NeuralNetworks/include/DifferentialNeuronWrapper.h>
#include <NeuralNetworks/include/ElectricalSynapsis.h>
#include <NeuralNetworks/include/HodgkinHuxleyModel.h>
#include <NeuralNetworks/include/RungeKutta4.h>

typedef RungeKutta4 Integrator;
typedef DifferentialNeuronWrapper<HodgkinHuxleyModel<double>, Integrator> HH;
typedef ElectricalSynapsis<HH, HH> Synapsis;

int main(int argc, char **argv) {
  // Struct to initialize neuron model parameters
  HH::ConstructorArgs args;

  // Set the parameter values
  args.cm = 1 * 7.854e-3;
  args.vna = 50;
  args.vk = -77;
  args.vl = -54.387;
  args.gna = 120 * 7.854e-3;
  args.gk = 36 * 7.854e-3;
  args.gl = 0.3 * 7.854e-3;

  // Initialize neuron models
  HH h1(args), h2(args);

  // Set initial value of V in neuron n1
  h1.set_variable(HH::v, -75);

  // Initialize a synapsis between the neurons
  Synapsis s(h1, HH::v, h2, HH::v, -0.002, -0.002);

  // Set the integration step
  const double step = 0.001;

  // Perform the simulation
  double simulation_time = 1000;
  for (double time = 0; t < simulation_time; time += step) {
    s.step(step);

    // Provide an external current input to both neurons
    h1.add_synaptic_input(0.5);
    h2.add_synaptic_input(0.5);

    h1.step(step);
    h2.step(step);

    printf("%f %f %f %f\n", time, h1.get_variable(HH::v),
           h2.get_variable(HH::v), s.get_variable(Synapsis::i1));
  }

  return 0;
}
