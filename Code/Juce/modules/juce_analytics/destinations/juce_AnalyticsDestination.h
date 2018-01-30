/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

//==============================================================================
/**
    An interface for handling analytics events collected by an Analytics object.

    For basic analytics logging you can implement this interface and add your
    class to an Analytics object.

    For more advanced logging you may want to subclass
    ThreadedAnalyticsDestination instead, which is more suitable for interacting
    with web servers and other time consuming destinations.

    @see Analytics, ThreadedAnalyticsDestination
*/
struct JUCE_API  AnalyticsDestination
{
    /** Contains information about an event to be logged. */
    struct AnalyticsEvent
    {
        /** The name of the event. */
        String name;

        /**
            The timestamp of the event.

            Timestamps are automatically applied by an Analytics object and are
            derived from Time::getMillisecondCounter(). As such these timestamps
            do not represent absolute times, but relative timings of events for
            each user in each session will be accurate.
        */
        uint32 timestamp;

        /** The parameters of the event. */
        StringPairArray parameters;

        /** The user ID associated with the event. */
        String userID;

        /** Properties associated with the user. */
        StringPairArray userProperties;
    };

    /** Constructor. */
    AnalyticsDestination() = default;

    /** Destructor. */
    virtual ~AnalyticsDestination() {}

    /**
        When an AnalyticsDestination is added to an Analytics object this method
        is called when an analytics event is triggered from the Analytics
        object.

        Override this method to log the event information somewhere useful.
    */
    virtual void logEvent (const AnalyticsEvent& event) = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyticsDestination)
};


} // namespace juce
