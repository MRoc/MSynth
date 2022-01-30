#include "MSampleBank.h"
#include <framework/io/MLogger.h>

const char* MSampleBank::CLASS_NAME = "MSampleBank";

MSampleBank::MSampleBank()
{
}

MSampleBank::~MSampleBank()
{
	MObjectList::deleteAll();
}

String MSampleBank::getClassName()
{
	return CLASS_NAME;
}

void MSampleBank::addSample( MSampleBankEntry* ptSample )
{
	MObjectList::add( ptSample );
}

void MSampleBank::removeSample( MSampleBankEntry* ptSample )
{
	MObjectList::remove( ptSample );
}

unsigned int MSampleBank::getSampleCount()
{
	return MObjectList::getLength();
}

MSampleBankEntry* MSampleBank::getSample( unsigned int index )
{
	return (MSampleBankEntry*) MObjectList::get( index );
}

void MSampleBank::import( String filename )
{
	MFileInputStream in;
	in.open( filename );
	
	char header[4];
	memset(header, 0, 4);
	in.read( (unsigned char*)header, 3 );

	unsigned int sampleCount = 0;

	if(strcmp(header, "MSB")==0)
	{
		sampleCount = 8;
	}
	else if(strcmp(header, "MS2")==0)
	{
		in.read( sampleCount );
	}

	std::vector<int> bufferSampleLength;
	for( unsigned int i=0;i<sampleCount;i++ )
	{
		MWave* tmpWav = new MWave();
		unsigned int tempLength;
		in.read( tempLength );
		bufferSampleLength.push_back( tempLength );
		this->addSample( new MSampleBankEntry( tmpWav ) );
	}

	std::vector<char> buffer;

	for(i=0;i<sampleCount;i++)
	{
		if( bufferSampleLength[ i ] )
		{
			buffer.clear();
			char cBuf;
			in.read( (unsigned char*)&cBuf, 1 );
			while(cBuf)
			{
				buffer.push_back(cBuf);
				in.read( (unsigned char*)&cBuf, 1 );
			}
			char *sampleName = new char[buffer.size()+1];
			for( unsigned int j=0;j<buffer.size();j++ )
			{
				sampleName[j] = buffer.at(j);
			}
			sampleName[j] = 0;
			getSample( i )->setName(sampleName);
			SAFE_DELETE( sampleName );
		}
		else
		{
			getSample( i )->setName( "" );
		}
	}

	for(i=0;i<sampleCount;i++)
	{
		MWave* tmpWav = getSample( i )->getWave();
		unsigned int samples = bufferSampleLength[ i ];
		if( samples )
		{
			unsigned int bytes = samples * sizeof(MW_SAMPLETYPE);
			MW_SAMPLETYPE* ptData = new MW_SAMPLETYPE[ samples ];
			in.read( (unsigned char*) ptData, bytes );
			tmpWav->setData( (unsigned char*) ptData, bytes );
		}
		else
		{
			tmpWav->setData( 0, 0 );
		}
	}

	in.close();
	bufferSampleLength.clear();
	buffer.clear();

	MLogger::logMessage( "<msamplebank::import bank=\"%s\">\n", filename.getData() );
	MLogger::logMessage( toString() );
	MLogger::logMessage( "</msamplebank::import>\n" );
}

void MSampleBank::export( String filename )
{
	MFileOutputStream out;

	out.open( filename );

	out.write("MS2", 3);
	unsigned int channelCount = MObjectList::getLength();
	out.write((char*)&channelCount, sizeof(channelCount));

	for( unsigned int i=0;i<channelCount;i++ )
	{
		int length;
		if( getSample( i )->getWave()->getData() )
		{
			length = getSample( i )->getWave()->getDataLength() / sizeof( MW_SAMPLETYPE );
		}
		else
		{
			length = 0;
		}
		out.write((char*)&length, sizeof(int));	
	}
	for(i=0;i<channelCount;i++)
	{
		String sampleName = getSample( i )->getName();
		if( sampleName.GetLength() > 0 ) 
		{
			out.write( sampleName.getData(), sampleName.GetLength()+1 );
			//out.Write((char*)&zero, sizeof( char ) );
		}
	}
	for(i=0;i<channelCount;i++)
	{
		MWave* ptWave = getSample( i )->getWave();
		if( ptWave && ptWave->getData() )
		{
			out.write(
				(unsigned char*)(ptWave->getData()),
				getSample( i )->getWave()->getDataLength() );
		}
	}

	out.close();
}

void MSampleBank::clearButLeaveSamples()
{
	for( unsigned int i=0;i<this->getLength();i++ )
		getSample( i )->setWave( 0 );
	MObjectList::deleteAll();
}

String MSampleBank::toString()
{
	String back;

	unsigned int count = this->getLength();
	for( unsigned int i=0;i<count;i++ )
		back += "\t<sample>" + getSample(i)->toString() + "</sample>\n";

	return back + "\n";
}