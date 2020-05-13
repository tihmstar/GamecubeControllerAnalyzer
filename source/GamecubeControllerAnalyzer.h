#ifndef GAMECUBECONTROLLER_ANALYZER_H
#define GAMECUBECONTROLLER_ANALYZER_H

#include <Analyzer.h>
#include "GamecubeControllerAnalyzerResults.h"
#include "GamecubeControllerSimulationDataGenerator.h"

#define FLAG_CONTROLLER (1 << 1)
#define FLAG_INVALID	  (1 << 2)


class GamecubeControllerAnalyzerSettings;
class ANALYZER_EXPORT GamecubeControllerAnalyzer : public Analyzer2
{
public:
	GamecubeControllerAnalyzer();
	virtual ~GamecubeControllerAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< GamecubeControllerAnalyzerSettings > mSettings;
	std::auto_ptr< GamecubeControllerAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	GamecubeControllerSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //GAMECUBECONTROLLER_ANALYZER_H
