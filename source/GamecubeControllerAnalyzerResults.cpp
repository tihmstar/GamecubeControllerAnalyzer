#include "GamecubeControllerAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "GamecubeControllerAnalyzer.h"
#include "GamecubeControllerAnalyzerSettings.h"
#include <iostream>
#include <fstream>

GamecubeControllerAnalyzerResults::GamecubeControllerAnalyzerResults( GamecubeControllerAnalyzer* analyzer, GamecubeControllerAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

GamecubeControllerAnalyzerResults::~GamecubeControllerAnalyzerResults()
{
}

void GamecubeControllerAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128+3] = {};
	memset(number_str, 0, sizeof(number_str));

	if (frame.mFlags & FLAG_CONTROLLER) {
		strcat(number_str, "S: ");
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str+3, 128 );
	}else{
		strcat(number_str, "M: ");
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str+3, 128 );
	}

	AddResultString( number_str );
}

void GamecubeControllerAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void GamecubeControllerAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128+3] = {};
	memset(number_str, 0, sizeof(number_str));

	if (frame.mFlags & FLAG_CONTROLLER) {
		strcat(number_str, "S: ");
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str+3, 128 );
	}else{
		strcat(number_str, "M: ");
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, frame.mData2, number_str+3, 128 );
	}

	AddTabularText( number_str );
#endif
}

void GamecubeControllerAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void GamecubeControllerAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}
