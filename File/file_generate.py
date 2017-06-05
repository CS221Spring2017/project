from random import randrange
import numpy
'''
array_mb = [4, 8, 16, 32, 64, 128, 256]
array_gb = [1, 2, 4]

for x in array_mb:
  fp = open("data/cache%sM" % x, "w+b")
  content = numpy.random.bytes(x*1024*1024)
  fp.write(content);
  fp.close()
for x in array_gb:
  fp = open("data/cache%sG" % x, "w+b")
  content = numpy.random.bytes(x*1024*1024*1024)
  fp.write(content);
  fp.close()
'''
for x in range(16):
	fp = open("data/file%d" % x, 'w+b')
	content = numpy.random.bytes(64*1024*1024)
	fp.write(content)
	fp.close()
