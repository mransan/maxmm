#ifndef MAXUTILS_LOGGER_H
#define MAXUTILS_LOGGER_H

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <ostream>
#include <fstream>
#include <map>

#include <maxutils/Mutex.h>

#define LOG_DEBUG   ::maxutils::Logger::debugConfig.setup  (__FILE__ , __LINE__); ::maxutils::Logger::debugStream()
#define LOG_INFO    ::maxutils::Logger::infoConfig.setup   (__FILE__ , __LINE__); ::maxutils::Logger::infoStream()
#define LOG_WARNING ::maxutils::Logger::warningConfig.setup(__FILE__ , __LINE__); ::maxutils::Logger::warningStream()
#define LOG_ERROR   ::maxutils::Logger::errorConfig.setup  (__FILE__ , __LINE__); ::maxutils::Logger::errorStream()


namespace maxutils
{    
    class Logger
    {
    public:
        enum Level
        {
            Debug,
            Info,
            Warning,
            Error,
            Fatal
        };
		
		static std::string LevelToString(Level l);
        
		typedef std::map<std::string, std::ostream *> StreamList;
        typedef StreamList::iterator                  StreamListItr;

        static StreamList streamList;

        static void init();
        static void flush();
        static void close();
        static void insertLogFile(const std::string& fileName);
        
        class LevelSink;
        class LevelConfig
        {
        friend class LevelSink;
        private:
            Level                       m_level;
            StreamList                  m_streamList;
            boost::filesystem::path     m_file;
            int                         m_line;
            boost::posix_time::ptime    m_time;
            bool                        m_reset;
            Mutex                       m_mutex;
        public:
            LevelConfig (Level _level)
            :m_level(_level)
            {
            }
            void            insertStream(const std::string& streamName);
            void            setup(const char  *_file, const int _line);
        };
        class LevelSink : public boost::iostreams::sink
        {
        private:
            LevelConfig& m_config;
        public:
            LevelSink(LevelConfig& _config)
            :m_config(_config)
            {
            
            }
            std::streamsize write(const char* s, std::streamsize n);
            
        };
        static LevelConfig debugConfig;
        static LevelConfig infoConfig;
        static LevelConfig warningConfig;
        static LevelConfig errorConfig;

        static boost::iostreams::stream<LevelSink>&  debugStream  (){ return m_debugStream; } ; 
        static boost::iostreams::stream<LevelSink>&  infoStream   (){ return m_infoStream; } ;
        static boost::iostreams::stream<LevelSink>&  warningStream(){ return m_warningStream; } ;
        static boost::iostreams::stream<LevelSink>&  errorStream  (){ return m_errorStream; } ;

        static boost::iostreams::stream<LevelSink> m_debugStream;
        static boost::iostreams::stream<LevelSink> m_infoStream;
        static boost::iostreams::stream<LevelSink> m_warningStream;
        static boost::iostreams::stream<LevelSink> m_errorStream;

    };

}//namespace utils

#endif
