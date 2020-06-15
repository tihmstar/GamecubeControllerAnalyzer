#include "GamecubeControllerAnalyzer.h"
#include "GamecubeControllerAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

GamecubeControllerAnalyzer::GamecubeControllerAnalyzer()
:	Analyzer2(),
	mSettings( new GamecubeControllerAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

GamecubeControllerAnalyzer::~GamecubeControllerAnalyzer()
{
	KillThread();
}

void GamecubeControllerAnalyzer::SetupResults()
{
	mResults.reset( new GamecubeControllerAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void GamecubeControllerAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();
	double SampleRateMHz = mSampleRateHz/1e6;

	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	if( mSerial->GetBitState() == BIT_LOW )
		mSerial->AdvanceToNextEdge();

	mSerial->AdvanceToNextEdge();
	mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel ); //starting marker

	int16_t prevbitcount = 0;
	int16_t bitcount = 0;
	U8 flag = 0;

	U32 fulldata = 0;
	U64 full_starting_sample = 0;


	printf("test\n");
	for( ; ; )
	{

		U8 data = 0xf; //invalid
		//we start at a falling edge
		U64 starting_sample = mSerial->GetSampleNumber();

		if (bitcount == 0) {
			full_starting_sample = starting_sample;
			fulldata = 0;
			flag = 0;
			prevbitcount = 0;
		}

		mSerial->AdvanceToNextEdge(); //we are at first rising edge now

		U64 first_rising = mSerial->GetSampleNumber();


		U64 low_diff = first_rising - starting_sample;
		double low_time = low_diff / SampleRateMHz;

		// printf("starting_sample=%lld first_rising=%lld low_diff=%lld low_time=%f SampleRateMHz=%f\n",starting_sample,first_rising,low_diff,low_time,SampleRateMHz);

		U64 final_falling = mSerial->GetSampleOfNextEdge();
		U64 full_diff = final_falling - starting_sample;
		double full_time = full_diff / SampleRateMHz;

		if (
				(bitcount % 8 == 0 && full_time > 7) /* console stop bit */ //expected 1.375us  /* controller stop bit */ //expected 4.5us
			){

			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if (flag & FLAG_INVALID) {
				printf("failed to decode frame\n");
			}else{
				//we have a byte to save.
				Frame frame;
				frame.mData1 = fulldata;
				frame.mFlags = flag;
				frame.mStartingSampleInclusive = full_starting_sample;
				frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

				mResults->AddFrame( frame );
				mResults->CommitResults();
				ReportProgress( frame.mEndingSampleInclusive );
			}



			bitcount = 0;
			mSerial->AdvanceToNextEdge(); //we are at next falling edge now
			continue;
		}

		mSerial->AdvanceToNextEdge(); //we are at next falling edge now


		U64 up_diff = final_falling - first_rising;
		double up_time = up_diff / SampleRateMHz;

		if (full_time >= 4.5/*4.5 us*/ && full_time <= 5 /*5 us*/) {
			//this is console

			if (up_time >= 0.5 && up_time <= 1.5) { //expected: 1.25us
				data = 0;
			}else if (low_time >= 0.5 && low_time <= 1.5 /*TODO: how long is a console 1 ?????*/) {
				data = 1;
			}else{
				data = 0xf; //invalid
				flag |= FLAG_INVALID;
			}
			bitcount++;
		}

		if (full_time >= 3.5 && full_time <= 4) {
			//this is controller
			flag |= FLAG_CONTROLLER;

			if (up_time >= 0.5 && up_time <= 1.5) { //expected: 1us
				data = 0;
			}else if (low_time >= 0.5 && low_time <= 1.5) { //expected: 1us
				data = 1;
			}else{
				data = 0xf; //invalid
				flag |= FLAG_INVALID;
			}

			bitcount++;
		}

		if (!bitcount) {
			continue; //this isn't a frame to decode
		}

		if (bitcount == prevbitcount) {
			//a frame failed to decode
			bitcount = 0;
		}

		prevbitcount = bitcount;
		fulldata <<=1;
		fulldata |= data;
	}
}

bool GamecubeControllerAnalyzer::NeedsRerun()
{
	return false;
}

U32 GamecubeControllerAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 GamecubeControllerAnalyzer::GetMinimumSampleRateHz()
{
	return 2*1000*1000 /*2MHz*/;
}

const char* GamecubeControllerAnalyzer::GetAnalyzerName() const
{
	return "Gamecube Controller";
}

const char* GetAnalyzerName()
{
	return "Gamecube Controller";
}

Analyzer* CreateAnalyzer()
{
	return new GamecubeControllerAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
