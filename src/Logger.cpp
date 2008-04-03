#include <maxutils/Logger.h>
#include <iomanip>
#include <iostream>

using namespace maxutils;

Logger::LevelConfig Logger::debugConfig(Logger::Debug);     
Logger::LevelConfig Logger::infoConfig(Logger::Info);       
Logger::LevelConfig Logger::warningConfig(Logger::Warning); 
Logger::LevelConfig Logger::errorConfig(Logger::Error);     

boost::iostreams::stream<Logger::LevelSink> Logger::m_debugStream(Logger::debugConfig);    
boost::iostreams::stream<Logger::LevelSink> Logger::m_infoStream(Logger::infoConfig);      
boost::iostreams::stream<Logger::LevelSink> Logger::m_warningStream(Logger::warningConfig);
boost::iostreams::stream<Logger::LevelSink> Logger::m_errorStream(Logger::errorConfig);    

Logger::StreamList Logger::streamList;


void Logger::init()
{
    Logger::streamList.insert(std::make_pair("cout", &std::cout));
    Logger::streamList.insert(std::make_pair("cerr", &std::cerr));
}

void Logger::flush()
{
    for ( Logger::StreamListItr it = Logger::streamList.begin() ;
          it                      != Logger::streamList.end();
          it ++
        )
    {
        it->second->flush();
    }

}

void Logger::close()
{
    
    Logger::flush();
    for ( Logger::StreamListItr it = Logger::streamList.begin() ;
          it                      != Logger::streamList.end();
          it ++
        )
    {
        if ( (it->first != "cout") && (it->first != "cerr"))
        {
            //it->second->close();
            delete it->second;
            it->second = 0;
        }
    }
    Logger::streamList.clear();
}

void Logger::insertLogFile(const std::string& fileName)
{
    std::ofstream* file = new std::ofstream();
    file->open(fileName.c_str(), std::ios_base::app);
    boost::posix_time::ptime    time = boost::posix_time::second_clock::local_time();

    *file 	<< std::endl
			<< std::endl
			<< "----------------------------------------------------------------------------------------------"
			<< std::endl
		    << "----------------------------         " 
            << time 
            << "         ----------------------------"
            << std::endl
            << "----------------------------------------------------------------------------------------------"
			<< std::endl
			<< std::endl;
    Logger::streamList.insert(std::make_pair(fileName , file));
}

std::string Logger::LevelToString(Logger::Level l)
{
	switch(l)
	{
		case Debug:
			return "[Debug]";
		case Warning:
			return "[Warning]";
		case Info:
			return "[Info]";
		case Error:
			return "[Error]";
	}
}
void Logger::LevelConfig::insertStream(const std::string& streamName)
{
    for ( Logger::StreamListItr it = Logger::streamList.begin() ;
          it                      != Logger::streamList.end();
          it ++
        )
    {
        if (it->first == streamName)
        {
            m_streamList.insert(std::make_pair(it->first, it->second));
        }
    }
}

void Logger::LevelConfig::setup(const std::string& _file, const int _line)
{
    m_file = basename(_file.c_str());
    m_line = _line;
    
    int lastSlashPos = m_file.rfind('/');
    if (m_file.size() - lastSlashPos > 30)
    {
        lastSlashPos = m_file.size() - 30;
    }
    //m_file.erase(0, lastSlashPos);

    m_time  = boost::posix_time::second_clock::local_time();
    m_reset = true;
}

std::streamsize Logger::LevelSink::write(const char* s, std::streamsize n)
{
    
    if (m_config.m_reset)
    {
        std::stringstream ss;
        
        ss  << m_config.m_time
            << " " 
            << std::setw(8)
			<< Logger::LevelToString(m_config.m_level)
			<< " "
			<< std::setw(20)
			<< std::setiosflags(std::ios::right)
			<< m_config.m_file
            << "@" 
            << std::setw(4)
			<< std::resetiosflags(std::ios::right)
			<< std::setiosflags(std::ios::left)
			<< m_config.m_line
            << ": ";
        ss.flush();
        for(Logger::StreamListItr it  = m_config.m_streamList.begin() ;
            it                       != m_config.m_streamList.end();
            it++
           )
        {
            it->second->write(ss.str().c_str() , ss.str().size());
        }
        m_config.m_reset = false;
    }


    
    for(Logger::StreamListItr it  = m_config.m_streamList.begin() ;
        it                       != m_config.m_streamList.end();
        it++
       )
    {
        it->second->write(s , n);
    }
    
    return n;
}

