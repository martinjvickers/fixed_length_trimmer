# fixed_length_trimmer
Trim a fastq or fastq.gz file to a fixed length

##Why, surely this has been done before?
Yes it has but not quite. First, this is not a quality trimmer. It doesn't account for quality at all, that's not its purpose as there are many available. Finding a quality trimmer that simply keeps the first N bases and discards the rest as well as ommitting any that are smaller than the specified length didn't appear to exist. Fastx-trimmer did appear to do what I want, but oddly when working with it I found that it would let reads through that are smaller than I had specified. So, I wrote this.

##Couldn't you do this with a script
Yup. I didn't want a script though, I wanted something more robust. The SeqAn library which this software is using is awesome when working with fastq's (and fastq.gz) out of the box.

##Limitations
*  Currently, it simply takes N bases starting from the left. I may add more functionality for flexible trimming in the future.
*  No matter what is in the third line of a read, it always simply displays a `+`. This is a property of the SeqAn library.

##Building from source 

This assumes you have cloned this library and the seqan library in the same directory along with ensuring you have all the dependancies that SeqAn relies upon)

    cd fixed_length_trimmer
    cmake ../fixed_length_trimmer -DCMAKE_MODULE_PATH=../seqan/util/cmake -DSEQAN_INCLUDE_PATH=../seqan/include -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Release
    make

##usage
    #to keep only the first 21 bases of each read. If a read is less than 21 bases, that read is discarded.
    ./fixed_len_trim -i inputfile.fq.gz -o output.fq -l 21
