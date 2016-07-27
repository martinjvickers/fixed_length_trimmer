#include "common.h"

struct ModifyStringOptions
{
        CharString inputFileName;
	CharString outputFileName;
	int length;
};

seqan::ArgumentParser::ParseResult parseCommandLine(ModifyStringOptions & options, int argc, char const ** argv)
{
	seqan::ArgumentParser parser("fixed_len_trim");
	addOption(parser, seqan::ArgParseOption("i", "input-file", "Path to the input file. Supported input: fq, fq.gz, fastq, fastq.gz, fasta, fasta.gz, fa and fa.gz.", seqan::ArgParseArgument::INPUT_FILE, "IN"));
	setRequired(parser, "input-file");
	setShortDescription(parser, "Methylation Tools");
	setVersion(parser, "0.0.1");
	setDate(parser, "July 2016");
	addUsageLine(parser, "-i [input file] -o [output file] -l [trim length]");
	addOption(parser, seqan::ArgParseOption("l", "length", "Length to trim to.",seqan::ArgParseArgument::INTEGER, "INT"));
	setRequired(parser, "l");
	addOption(parser, seqan::ArgParseOption("o", "output-file", "Path to the output file. You must include a file extension. Supported output types: fq, fastq, fasta and fa.", seqan::ArgParseArgument::OUTPUT_FILE, "OUT"));
	setRequired(parser, "o");

	addDescription(parser, "Trims your fasta files to a fixed length.");
	seqan::ArgumentParser::ParseResult res = seqan::parse(parser, argc, argv);

	// Extract options ONLY if the args are parsed correctly
	if (res != seqan::ArgumentParser::PARSE_OK)
		return res;

	getOptionValue(options.inputFileName, parser, "input-file");
	getOptionValue(options.length, parser, "length");
	getOptionValue(options.outputFileName, parser, "output-file");

	return seqan::ArgumentParser::PARSE_OK;

}

int main(int argc, char const ** argv)
{
	//parse our options
	ModifyStringOptions options;
	seqan::ArgumentParser::ParseResult res = parseCommandLine(options, argc, argv);

	// If parsing was not successful then exit with code 1 if there were errors.
	// Otherwise, exit with code 0 (e.g. help was printed).
	if (res != seqan::ArgumentParser::PARSE_OK)
		return res == seqan::ArgumentParser::PARSE_ERROR;	


	SeqFileIn seqFileIn;
	if (!open(seqFileIn, toCString(options.inputFileName)))
	{
		std::cerr << "ERROR: Could not open the file.\n";
		return 1;
	}

	CharString id;
	Dna5String seq;
	CharString qual;

	SeqFileOut seqFileOut;
        if (!open(seqFileOut, toCString(options.outputFileName)))
        {
                std::cerr << "ERROR: Could not open the file.\n";
                return 1;
        }

	while(!atEnd(seqFileIn))
	{
        	try
        	{
        	        readRecord(id, seq, qual, seqFileIn);
        	}
        	catch (Exception const & e)
        	{
        	        std::cout << "ERROR: " << e.what() << std::endl;
        	        return 1;
        	}

               	if(length(seq) >= options.length)
               	{
                       	String<Dna> dnaSeq;
               	        resize(dnaSeq, options.length, Exact());
               	        assign(dnaSeq, seq, Limit());

               	        CharString dnaQual;
               	        resize(dnaQual, options.length, Exact());
               	        assign(dnaQual, qual, Limit());

			writeRecord(seqFileOut, id, dnaSeq, dnaQual);			
		}
	}
	
	return 0;
}
