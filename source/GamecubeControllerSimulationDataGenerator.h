#ifndef GAMECUBECONTROLLER_SIMULATION_DATA_GENERATOR
#define GAMECUBECONTROLLER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class GamecubeControllerAnalyzerSettings;

class GamecubeControllerSimulationDataGenerator
{
public:
	GamecubeControllerSimulationDataGenerator();
	~GamecubeControllerSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, GamecubeControllerAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	GamecubeControllerAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //GAMECUBECONTROLLER_SIMULATION_DATA_GENERATOR
