# JMidiTrigger

A JUCE based vst allowing to convert incoming MIDI notes into other MIDI data based on a definitions loaded from a XML file.

Currently based on JUCE 4.1.0.

## JUCE 

### as git submodule.

Example to switch the JUCE version:

```sh
cd JUCE
git remote set-branches origin develop
git fetch --depth 1 origin develop
git checkout develop
```

### as subtree

```sh

# define as remote to shorten other commands
git remote add -f juce https://github.com/juce-framework/JUCE.git

# create subtree
git subtree add --prefix=source/vendor/juce juce 4.1.0 --squash

# Update to a tagged revision:
git subtree pull --prefix=source/vendor/juce juce 4.1.0 --squash

# Update to the newest revision:
git subtree pull --prefix=source/vendor/juce juce master --squash

# later updates
git fetch juce master
git subtree pull -prefix=source/vendor/juce juce master --squash
```