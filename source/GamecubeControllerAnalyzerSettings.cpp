#include "GamecubeControllerAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


GamecubeControllerAnalyzerSettings::GamecubeControllerAnalyzerSettings()
	:	mInputChannel( UNDEFINED_CHANNEL )
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "Data wire", "Standard Gamecube Controller" );
	mInputChannelInterface->SetChannel( mInputChannel );


	AddInterface( mInputChannelInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Serial", false );
}

GamecubeControllerAnalyzerSettings::~GamecubeControllerAnalyzerSettings()
{
}

bool GamecubeControllerAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mInputChannel, "Gamecube Controller", true );

	return true;
}

void GamecubeControllerAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
}

void GamecubeControllerAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;

	ClearChannels();
	AddChannel( mInputChannel, "Gamecube Controller", true );

	UpdateInterfacesFromSettings();
}

const char* GamecubeControllerAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;

	return SetReturnString( text_archive.GetString() );
}
