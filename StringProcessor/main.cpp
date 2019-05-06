#include <iostream>
#include <string>

#include "SequentialPipeline.h"
#include "ReaderStage.h"
#include "ProcessStage.h"
#include "WriterStage.h"
#include "StringProcSet.h"

int main()
{
	//Change absolut paths to input/output files 
	std::string input_filename = 
		"";
	std::string output_filename = 
		"";

	SequentialPipeline<std::string> ppl;

// Reader Stage - Stage 0
/*-----------------------------------------------------------*/
	ppl.AddStage(
		std::shared_ptr<ReaderStage>(new ReaderStage(0, input_filename))
	);

// Processing Stage 1
/*-----------------------------------------------------------*/
	ppl.AddStage(
		std::shared_ptr<ProcessStage>(new ProcessStage(1))
	);

	// Add operations on Stage 1
	ppl.AddOperationOnStage(1, StringProcSet::Uppercase);
	ppl.AddOperationOnStage(1, StringProcSet::RemoveSpaces);

// Processing Stage 2
/*-----------------------------------------------------------*/
	ppl.AddStage(
		std::shared_ptr<ProcessStage>(new ProcessStage(2))
	);

	// Add operations on Stage 2
	ppl.AddOperationOnStage(2, StringProcSet::Reverse);

// Processing Stage 3
/*-----------------------------------------------------------*/
	ppl.AddStage(
		std::shared_ptr<ProcessStage>(new ProcessStage(3))
	);

	// Add operations on Stage 3
	ppl.AddOperationOnStage(3, StringProcSet::SortAscending);

// Writer Stage - Stage 4
/*-----------------------------------------------------------*/
	ppl.AddStage(
		std::shared_ptr<WriterStage>(new WriterStage(4, output_filename))
	);
	
	ppl.Start();
	ppl.Join();

	//ppl.PrintResult();

	return 0;
}