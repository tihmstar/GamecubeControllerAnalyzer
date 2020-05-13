#ifndef GAMECUBECONTROLLER_ANALYZER_SETTINGS
#define GAMECUBECONTROLLER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class GamecubeControllerAnalyzerSettings : public AnalyzerSettings
{
public:
	GamecubeControllerAnalyzerSettings();
	virtual ~GamecubeControllerAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();


	Channel mInputChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //GAMECUBECONTROLLER_ANALYZER_SETTINGS
