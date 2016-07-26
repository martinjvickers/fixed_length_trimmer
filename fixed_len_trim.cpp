#include "common.h"

struct ModifyStringOptions
{
        CharString inputFileName;
	CharString outputFileName;
	int length;
};

seqan::ArgumentParser::ParseResult parseCommandLine(ModifyStringOptions & options, int argc, char const ** argv)
{
	seqan::ArgumentParser parser("Fixed Length Trimmer");
	addOption(parser, seqan::ArgParseOption("i", "input-file", "Path to the input file", seqan::ArgParseArgument::INPUT_FILE, "IN"));
	setRequired(parser, "input-file");
	setShortDescription(parser, "Methylation Tools");
	setVersion(parser, "0.0.1");
	setDate(parser, "July 2016");
	addUsageLine(parser, "-i sequence.fastq [\\fIOPTIONS\\fP] ");
	addOption(parser, seqan::ArgParseOption("l", "length", "Length to trim to.",seqan::ArgParseArgument::INTEGER, "INT"));
	setDefaultValue(parser, "length", "21");
	addOption(parser, seqan::ArgParseOption("o", "output-file", "Path to the output file", seqan::ArgParseArgument::OUTPUT_FILE, "IN"));

	addDescription(parser, "A tool to quickly deal with Bismark downstream analysis in a version controlled stable way");
	seqan::ArgumentParser::ParseResult res = seqan::parse(parser, argc, argv);

	// If parsing was not successful then exit with code 1 if there were errors.
	// Otherwise, exit with code 0 (e.g. help was printed).
	if (res != seqan::ArgumentParser::PARSE_OK)
		return res;

	getOptionValue(options.inputFileName, parser, "input-file");
	getOptionValue(options.length, parser, "length");
	getOptionValue(options.outputFileName, parser, "output-file");

	return seqan::ArgumentParser::PARSE_OK;

}

/*
Aim: calculate introns.

Current progress: It compiles!

*/
int main(int argc, char const ** argv)
{
	//parse our options
	ModifyStringOptions options;
	seqan::ArgumentParser::ParseResult res = parseCommandLine(options, argc, argv);
	
	SeqFileIn seqFileIn;
	if (!open(seqFileIn, toCString(options.inputFileName)))
	{
		std::cerr << "ERROR: Could not open the file.\n";
		return 1;
	}


	StringSet<CharString> ids;
	StringSet<Dna5String> seqs;
	StringSet<CharString> quals;

	try
	{
		readRecords(ids, seqs, quals, seqFileIn);
	}
	catch (Exception const & e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}	

	for (unsigned i = 0; i < length(ids); ++i)
		if(length(seqs[i]) >= options.length)
		{
			String<Dna> dnaSeq;
			resize(dnaSeq, options.length, Exact());
			assign(dnaSeq, seqs[i], Limit());

			CharString dnaQual;
			resize(dnaQual, options.length, Exact());
			assign(dnaQual, quals[i], Limit());
			
			std::cout << "@" << ids[i] << "\n";
			std::cout << dnaSeq << "\n";
			std::cout << "+" << "\n";
			std::cout << dnaQual << "\n";
		}

	return 0;
}
