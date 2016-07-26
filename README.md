# fixed_length_trimmer
Trim a fastq or fastq.gz file to a fixed length

##Why, surely this has been done before?
Yes it has but not quite. First, this is not a quality trimmer. It doesn't account for quality at all, that's not its purpose as there are many available. Finding a quality trimmer that simply keeps the first N bases and discards the rest as well as ommitting any that are smaller than the specified length didn't appear to exist. Fastx-trimmer did appear to do what I want, but oddly when working with it I found that it would let reads through that are smaller than I had specified. So, I wrote this.

##Couldn't you do this with a script
Yup. I didn't want a script though, I wanted something more robust. SeqAn is awesome when working with fastq's (and fastq.gz) out of the box.

##Limitations
*  Currently, it simply takes N bases starting from the left. 
*  No matter what is in the third line of a read, it always simply displays a `+`
*  It prints to stdout only at the moment.

##Building from source (assuming you have cloned this library and the seqan library in the same directory)
    cd fixed_length_trimmer
    cmake ../fixed_length_trimmer -DCMAKE_MODULE_PATH=../seqan/util/cmake -DSEQAN_INCLUDE_PATH=../seqan/include -DCMAKE_CXX_FLAGS=-std=c++11 -DCMAKE_BUILD_TYPE=Release
    make

##usage
    ./fixed_len_trim -i example_data/lane_5_p1.fastq.gz -l 100 > example_data/lane_5__p1_trimmed.fastq
