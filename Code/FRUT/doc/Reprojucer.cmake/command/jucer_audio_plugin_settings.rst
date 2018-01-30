jucer_audio_plugin_settings
===========================

Define the settings specific to an *Audio Plug-in* project.

::

  jucer_audio_plugin_settings(
    [BUILD_VST <ON|OFF>]
    [BUILD_VST3 <ON|OFF>]
    [BUILD_AUDIOUNIT <ON|OFF>]
    [BUILD_AUDIOUNIT_V3 <ON|OFF>]
    [BUILD_RTAS <ON|OFF>]
    [BUILD_AAX <ON|OFF>]
    [BUILD_STANDALONE_PLUGIN <ON|OFF>]
    [PLUGIN_NAME <plugin_name>]
    [PLUGIN_DESCRIPTION <plugin_description>]
    [PLUGIN_MANUFACTURER <plugin_manufacturer>]
    [PLUGIN_MANUFACTURER_CODE <plugin_manufacturer_code>]
    [PLUGIN_CODE <plugin_code>]
    [PLUGIN_CHANNEL_CONFIGURATIONS <plugin_channel_configurations>]
    [PLUGIN_IS_A_SYNTH <ON|OFF>]
    [PLUGIN_MIDI_INPUT <ON|OFF>]
    [PLUGIN_MIDI_OUTPUT <ON|OFF>]
    [MIDI_EFFECT_PLUGIN <ON|OFF>]
    [KEY_FOCUS <ON|OFF>]
    [PLUGIN_AU_EXPORT_PREFIX <plugin_au_export_prefix>]
    [PLUGIN_AU_MAIN_TYPE <plugin_au_main_type>]
    [VST_CATEGORY <vst_category>]
    [PLUGIN_RTAS_CATEGORY <plugin_rtas_category>]
    [PLUGIN_AAX_CATEGORY <plugin_aax_category>]
    [PLUGIN_AAX_IDENTIFIER <plugin_aax_identifier>]
  )

You must call this command when you call `jucer_project_settings()
<jucer_project_settings.rst>`_ with ``PROJECT_TYPE "Audio Plug-in"``.
