
#include "FileUtils.h"

namespace FileUtils {

	juce::String getRelativeFilePath(const juce::File& fi)
	{
		if (!fi.exists())
		{
			//logger.log("Error - file does not exist: " + fi.getFullPathName(),1);
			return juce::String("");
		}
		else
		{
			const auto applicationDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory();
			juce::String relativeXmlFilePath = fi.getRelativePathFrom(applicationDir);
			//logger.log(">>> relative path to config file: " + relativeXmlFilePath,1);
			//setStateValue(Identifier("xmlFilePath"), xmlFilePath.getValue().toString());
			//debug("after setstatevalue");

			return relativeXmlFilePath;

		}
	}

	juce::String solveRelativeFilePath(const juce::String& path)
	{
		if (juce::File::isAbsolutePath(path))
		{
			return path;
		}
		else
		{
			// to allow for use of both absolute and relative paths, first resolve working dir.
			// getChildFile will handle absolute paths as well, so no need to check if rel or abs.
			const auto applicationDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory();
			const auto& resolvedFile = applicationDir.getChildFile(path);
			return resolvedFile.getFullPathName();
		}
	}

}
