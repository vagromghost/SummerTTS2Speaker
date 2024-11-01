# SummerTTS 用于纪念2023年即将到来和终将逝去的夏天


通过 `SummerTTS` 文本转语音,直接输出到声音设备，不再存储文件.

# 新的使用说明



1. 编译环境

   ```
   操作系统： Ubuntu 20.04.5 LTS
   ```

   

1. 安装 `portaudio` 依赖

   ```
   apt install portaudio19-dev
   ```

   

2. 查看 `portaudio` 安装位置

   ```
   dpkg -L portaudio19-dev
   ```

   返回示例:

   ```
   .
   /usr
   /usr/include
   /usr/include/pa_jack.h
   /usr/include/pa_linux_alsa.h
   /usr/include/pa_unix_oss.h
   /usr/include/portaudio.h
   /usr/include/portaudiocpp
   /usr/include/portaudiocpp/AutoSystem.hxx
   /usr/include/portaudiocpp/BlockingStream.hxx
   /usr/include/portaudiocpp/CFunCallbackStream.hxx
   /usr/include/portaudiocpp/CallbackInterface.hxx
   /usr/include/portaudiocpp/CallbackStream.hxx
   /usr/include/portaudiocpp/CppFunCallbackStream.hxx
   /usr/include/portaudiocpp/Device.hxx
   /usr/include/portaudiocpp/DirectionSpecificStreamParameters.hxx
   /usr/include/portaudiocpp/Exception.hxx
   /usr/include/portaudiocpp/HostApi.hxx
   /usr/include/portaudiocpp/InterfaceCallbackStream.hxx
   /usr/include/portaudiocpp/MemFunCallbackStream.hxx
   /usr/include/portaudiocpp/PortAudioCpp.hxx
   /usr/include/portaudiocpp/SampleDataFormat.hxx
   /usr/include/portaudiocpp/Stream.hxx
   /usr/include/portaudiocpp/StreamParameters.hxx
   /usr/include/portaudiocpp/System.hxx
   /usr/include/portaudiocpp/SystemDeviceIterator.hxx
   /usr/include/portaudiocpp/SystemHostApiIterator.hxx
   /usr/lib
   /usr/lib/aarch64-linux-gnu
   /usr/lib/aarch64-linux-gnu/libportaudio.a
   /usr/lib/aarch64-linux-gnu/libportaudiocpp.a
   /usr/lib/aarch64-linux-gnu/pkgconfig
   /usr/lib/aarch64-linux-gnu/pkgconfig/portaudio-2.0.pc
   /usr/lib/aarch64-linux-gnu/pkgconfig/portaudiocpp.pc
   /usr/share
   /usr/share/doc
   /usr/share/doc/portaudio19-dev
   /usr/share/doc/portaudio19-dev/copyright
   /usr/lib/aarch64-linux-gnu/libportaudio.so
   /usr/lib/aarch64-linux-gnu/libportaudiocpp.so
   /usr/share/doc/portaudio19-dev/changelog.Debian.gz
   ```

   

3. 修改根据目录下的 `CMakeLists.txt` 文件

   在文件中找到 `target_include_directories` 增加一个 `/usr/include` 目录：

   ```
   target_include_directories(tts_test PUBLIC ./eigen-3.4.0
                                               ./src/tn/header
                                                 ./include
                                                 ./src/header
   					      															/usr/include)
   ```

   在文件最后增加：

   ```
   target_link_libraries(tts_test PRIVATE /usr/lib/aarch64-linux-gnu/libportaudio.so)
   ```

   最终文件内容是：

   ```
   cmake_minimum_required(VERSION 3.5)
   
   project(tts_test)
   
   #set(CMAKE_CXX_FLAGS " -O3 -w -std=c++11 ")
   #set(CMAKE_C_FLAGS " -O3 -w -std=c++11 ")
   
   set(CMAKE_CXX_FLAGS " -O3 -fopenmp -std=c++11 ")
   set(CMAKE_C_FLAGS " -O3 -fopenmp -std=c++11 ")
   
   add_executable(tts_test
                  ./test/main.cpp
                  ./src/tn/glog/src/demangle.cc
                  ./src/tn/glog/src/logging.cc
                  ./src/tn/glog/src/raw_logging.cc
                  ./src/tn/glog/src/symbolize.cc
                  ./src/tn/glog/src/utilities.cc
                  ./src/tn/glog/src/vlog_is_on.cc
                  ./src/tn/glog/src/signalhandler.cc
                  ./src/tn/gflags/src/gflags.cc
                  ./src/tn/gflags/src/gflags_reporting.cc
                  ./src/tn/gflags/src/gflags_completions.cc
                  ./src/tn/openfst/src/lib/compat.cc
                  ./src/tn/openfst/src/lib/flags.cc
                  ./src/tn/openfst/src/lib/fst.cc
                  ./src/tn/openfst/src/lib/fst-types.cc
                  ./src/tn/openfst/src/lib/mapped-file.cc
                  ./src/tn/openfst/src/lib/properties.cc
                  ./src/tn/openfst/src/lib/symbol-table.cc
                  ./src/tn/openfst/src/lib/symbol-table-ops.cc
                  ./src/tn/openfst/src/lib/util.cc
                  ./src/tn/openfst/src/lib/weight.cc
                  ./src/tn/processor.cc
                  ./src/tn/token_parser.cc
                  ./src/tn/utf8_string.cc
                  ./src/engipa/EnglishText2Id.cpp
                  ./src/engipa/InitIPASymbols.cpp
                  ./src/engipa/alphabet.cpp
                  ./src/engipa/ipa.cpp
                  ./src/hz2py/hanzi2phoneid.cpp
                  ./src/hz2py/Hanz2Piny.cpp
                  ./src/hz2py/pinyinmap.cpp
                  ./src/nn_op/nn_conv1d.cpp
                  ./src/nn_op/nn_softmax.cpp
                  ./src/nn_op/nn_layer_norm.cpp
                  ./src/nn_op/nn_relu.cpp
                  ./src/nn_op/nn_gelu.cpp
                  ./src/nn_op/nn_tanh.cpp
                  ./src/nn_op/nn_flip.cpp
                  ./src/nn_op/nn_cumsum.cpp
                  ./src/nn_op/nn_softplus.cpp
                  ./src/nn_op/nn_clamp_min.cpp
                  ./src/nn_op/nn_sigmoid.cpp
                  ./src/nn_op/nn_conv1d_transposed.cpp
                  ./src/nn_op/nn_leaky_relu.cpp
                  ./src/platform/tts_file_io.cpp
                  ./src/platform/tts_logger.cpp
                  ./src/utils/utils.cpp
                  ./src/modules/iStft.cpp
                  ./src/modules/hann.cpp
                  ./src/modules/attention_encoder.cpp
                  ./src/modules/multi_head_attention.cpp
                  ./src/modules/ffn.cpp
                  ./src/modules/ConvFlow.cpp
                  ./src/modules/DDSConv.cpp
                  ./src/modules/ElementwiseAffine.cpp
                  ./src/modules/random_gen.cpp
                  ./src/modules/ResidualCouplingLayer.cpp
                  ./src/modules/ResBlock1.cpp
                  ./src/modules/WN.cpp
                  ./src/modules/pqmf.cpp
                  ./src/models/TextEncoder.cpp
                  ./src/models/StochasticDurationPredictor.cpp
                  ./src/models/FixDurationPredictor.cpp
                  ./src/models/DurationPredictor_base.cpp
                  ./src/models/ResidualCouplingBlock.cpp
                  ./src/models/Generator_base.cpp
                  ./src/models/Generator_hifigan.cpp
                  ./src/models/Generator_MS.cpp
                  ./src/models/Generator_Istft.cpp
                  ./src/models/Generator_MBB.cpp
                  ./src/models/SynthesizerTrn.cpp)
   
   target_include_directories(tts_test PUBLIC ./eigen-3.4.0
                                               ./src/tn/header
                                                 ./include
                                                 ./src/header
   					      															/usr/include)
   target_link_libraries(tts_test PRIVATE /usr/lib/aarch64-linux-gnu/libportaudio.so)
   
   
   ```

   

4. 进入 `Build` 目录，执行以下命令：  

   ```
   cmake ..  
   make
   ```

   

5. 运行

   ```
   ./tts_test 我是中国人 ../models/single_speaker_fast.bin
   ```




# 参考

  - SummerTTS (https://github.com/huakunyang/SummerTTS)  
  - wavplayer (https://github.com/hosackm/wavplayer)  
  - portaudio (https://github.com/PortAudio/portaudio)  

