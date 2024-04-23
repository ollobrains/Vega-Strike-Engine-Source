/*
 * vs_logging.cpp
 *
 * Copyright (C) 2001-2024 Daniel Horn, Stephen G. Tuggy and
 * other Vega Strike contributors.
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */


#include "vs_logging.h"

#include <string>
#include <cstdint>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/filesystem.hpp>

namespace VegaStrikeLogging {

// void exitProgram(int code)
// {
//     Music::CleanupMuzak();
//     VegaStrikeLogging::VegaStrikeLogger::instance().FlushLogs();
//     winsys_exit(code);
// }

void VegaStrikeLogger::InitLoggingPart2(const uint8_t debug_level,
        const boost::filesystem::path &vega_strike_home_dir) {

    const boost::filesystem::path &logging_dir = boost::filesystem::absolute("logs",
            vega_strike_home_dir);         /*< $HOME/.vegastrike/logs, typically >*/
    const std::string &logging_dir_name = logging_dir.string();
    VS_LOG(info, (boost::format("log directory : '%1%'") % logging_dir_name));

    switch (debug_level) {
        case 1:
            logging_core_->set_filter(severity >= info);
            break;
        case 2:
            logging_core_->set_filter(severity >= debug);
            break;
        case 3:
            logging_core_->set_filter(severity >= trace);
            break;
        default:
            logging_core_->set_filter(severity >= important_info);
            break;
    }

    boost::shared_ptr<FileLogBackEnd> file_log_back_end_ = boost::make_shared<FileLogBackEnd>
            (
                    boost::log::keywords::file_name =
                            logging_dir_name + "/" + "vegastrike_%Y-%m-%d_%H_%M_%S.%f.log", /*< file name pattern >*/
                    boost::log::keywords::rotation_size = 10 * 1024
                            * 1024,                                               /*< rotate files every 10 MiB... >*/
                    boost::log::keywords::time_based_rotation =
                            boost::log::sinks::file::rotation_at_time_point(0, 0, 0),     /*< ...or at midnight >*/
                    boost::log::keywords::format =
                            "[%TimeStamp%]: %Message%",                                     /*< log record format >*/
                    boost::log::keywords::auto_flush =
                            false, /*true,*/                                                /*< whether to auto flush to the file after every line >*/
                    boost::log::keywords::min_free_space = 2UL * 1024UL * 1024UL
                            * 1024UL                                      /*< stop boost::log when there's only 2 GiB free space left >*/
            );
    file_log_sink_ = boost::make_shared<FileLogSink>(file_log_back_end_);
    logging_core_->add_sink(file_log_sink_);

    console_log_sink_->set_filter(severity >= fatal);
}

void VegaStrikeLogger::FlushLogs() {
    if (console_log_sink_) {
        console_log_sink_->feed_records();
        console_log_sink_->flush();
    }
    if (file_log_sink_) {
        file_log_sink_->feed_records();
        file_log_sink_->flush();
    }
    std::cout << std::flush;
    std::cerr << std::flush;
    std::clog << std::flush;
}

void VegaStrikeLogger::FlushLogsProgramExiting() {
    if (console_log_sink_) {
        logging_core_->remove_sink(console_log_sink_);
        console_log_sink_->stop();
        console_log_sink_->feed_records();
        console_log_sink_->flush();
    }
    if (file_log_sink_) {
        logging_core_->remove_sink(file_log_sink_);
        file_log_sink_->stop();
        file_log_sink_->feed_records();
        file_log_sink_->flush();
    }

    std::cout << std::flush;
    std::cerr << std::flush;
    std::clog << std::flush;

    console_log_sink_.reset();
    file_log_sink_.reset();
}

VegaStrikeLogger::VegaStrikeLogger() : slg_(), file_log_sink_(nullptr) {
    logging_core_ = boost::log::core::get();
    boost::log::add_common_attributes();

    boost::shared_ptr<ConsoleLogBackEnd> console_log_back_end_ = boost::make_shared<ConsoleLogBackEnd>
            (
                    boost::shared_ptr<std::ostream>(&std::cerr, boost::null_deleter()),
                    boost::log::keywords::format =
                            "%Message%",                                                    /*< log record format specific to the console >*/
                    boost::log::keywords::auto_flush =
                            false /*true*/                                                  /*< whether to do the equivalent of fflush(stdout) after every msg >*/
            );
    console_log_sink_ = boost::make_shared<ConsoleLogSink>(console_log_back_end_);
    logging_core_->add_sink(console_log_sink_);
}

VegaStrikeLogger::~VegaStrikeLogger() {
    FlushLogsProgramExiting();
}

void VegaStrikeLogger::Log(const vega_log_level level, const std::string &message) {
    boost::log::record rec = slg_.open_record(boost::log::keywords::severity = level);
    if (rec)
    {
        boost::log::record_ostream strm(rec);
        strm << message;
        strm.flush();
        slg_.push_record(boost::move(rec));
    }
}

void VegaStrikeLogger::LogAndFlush(const vega_log_level level, const std::string &message) {
    Log(level, message);
    FlushLogs();
}

void VegaStrikeLogger::LogFlushExit(const vega_log_level level, const std::string& message, const int exit_code = -50) {
    Log(level, message);
    FlushLogsProgramExiting();
    exit(exit_code);
}

void VegaStrikeLogger::Log(const vega_log_level level, const char *message) {
    boost::log::record rec = slg_.open_record(boost::log::keywords::severity = level);
    if (rec)
    {
        boost::log::record_ostream strm(rec);
        strm << message;
        strm.flush();
        slg_.push_record(boost::move(rec));
    }
}

void VegaStrikeLogger::LogAndFlush(const vega_log_level level, const char *message) {
    Log(level, message);
    FlushLogs();
}

void VegaStrikeLogger::LogFlushExit(const vega_log_level level, const char* message, const int exit_code = -50) {
    Log(level, message);
    FlushLogsProgramExiting();
    exit(exit_code);
}

void VegaStrikeLogger::Log(const vega_log_level level, const boost::basic_format<char> &message) {
    boost::log::record rec = slg_.open_record(boost::log::keywords::severity = level);
    if (rec)
    {
        boost::log::record_ostream strm(rec);
        strm << message;
        strm.flush();
        slg_.push_record(boost::move(rec));
    }
}

void VegaStrikeLogger::LogAndFlush(const vega_log_level level, const boost::basic_format<char> &message) {
    Log(level, message);
    FlushLogs();
}

void VegaStrikeLogger::LogFlushExit(const vega_log_level level, const boost::basic_format<char>& message, const int exit_code = -50) {
    Log(level, message);
    FlushLogsProgramExiting();
    exit(exit_code);
}

} // namespace VegaStrikeLogging
