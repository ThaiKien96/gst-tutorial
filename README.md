# gst-tutorial
This repo use to store the tutorials of gstreamer which Kien learnt.

## Introduce
GStreamer is an open-source, pipeline-based multimedia framework for handling audio/video streaming, processing, and playback. Its modular architecture allows developers to build complex media workflows by connecting reusable components. Here’s a breakdown of its core components:

## Structure
### 1. Core Building Blocks
#### Elements:
* Atomic units performing specific tasks (e.g., reading files, decoding, rendering).
* Types: Source (data producers), Sink (data consumers), Filter (transform data), Demuxer/Muxer (split/combine streams).
* Example: filesrc (read file), h264parse (parse H.264), autovideosink (render video).
#### Pads:
* Connection points on elements for data flow.
* Source Pads (output) → Send data.
* Sink Pads (input) → Receive data.
* Negotiation: Pads agree on supported data formats (e.g., video resolution, codec) via capabilities (caps).
#### Bins:
* Containers grouping multiple elements into a single logical unit.
* Simplify complex pipelines (e.g., a "playbin" bin for playback).
#### Pipeline:
* Top-level bin controlling data flow, synchronization, and messaging.
* Manages the GStreamer Bus for event handling (e.g., errors, state changes).
### 2. Data Flow & Processing
#### Buffers:
Carry chunks of raw media data (e.g., video frames, audio samples).
#### Events:
Control messages (e.g., seeking, end-of-stream, caps changes).
#### Clock:
Synchronizes playback across elements for A/V sync.
### 3. Pipeline Execution
#### Element States:
```css
NULL → READY → PAUSED → PLAYING (transitions control resource allocation/data flow).
```
#### Data Flow:
* Downstream: Buffers flow from source to sink elements.
* Upstream: Events (e.g., seek requests) propagate backward.
* Negotiation: Elements agree on data formats (caps) via pad queries before streaming.
### 4. Key Infrastructure
#### Plugins:
* Dynamically loadable modules containing element implementations.
* Core Plugins: Provide essential elements (e.g., coreelements).
#### Codec Plugins:
* Add support for formats (e.g., gst-plugins-good, -bad, -ugly).
#### GStreamer Bus:
* Asynchronous message queue for pipeline events (e.g., errors, state changes).
#### Registry:
* Database of available plugins/elements loaded at runtime.
### 5. Advanced Features
#### Dynamic Pipelines:
* Modify pipelines at runtime (e.g., add/remove elements for adaptive streaming).
#### Controller:
* Animate element properties (e.g., volume fade).
* Hardware Acceleration:
* Integration with APIs like VA-API, NVDEC, Vulkan.
#### Debugging:

* Tools like GST_DEBUG logs and gst-launch-1.0 CLI for pipeline prototyping.

### Example Pipeline

```bash
# Play a video file using gst-launch:
gst-launch-1.0 filesrc location=video.mp4 ! \
qtdemux name=demux \
demux.video_0 ! queue ! h264parse ! avdec_h264 ! videoconvert ! autovideosink \
demux.audio_0 ! queue ! aacparse ! avdec_aac ! audioconvert ! autoaudiosink
```
Steps: Read file → Demux streams → Decode → Convert → Render.