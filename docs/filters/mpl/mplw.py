#!/usr/bin/env python

import os, sys
from optparse import *

#from matplotlib.pyplot import barh, title, grid , savefig, yticks, xlabel

from matplotlib.pyplot import *
from numpy  import *

import csv
import string

class EApp(Exception):
    '''Application specific exception.'''
    pass

class Application():
    '''
OPTIONS
    -s OUTFILE, --style=OUTFILE
        plot style 

    -o OUTFILE, --outfile=OUTFILE
        The file name of the output file. If not specified the output file is
        named like INFILE but with a .png file name extension.

    -v, --verbose
        Verbosely print processing information to stderr.

    '''

    def __init__(self, argv=None):
        if not argv:
            argv = sys.argv

        self.usage = '%prog [options] inputfile'
        self.option_list = [
            Option("-o", "--outfile", action="store",
            dest="outfile",
            help="Output file"),
            Option("-s", "--style", action="store",
                    dest="style", default="asciidoc", type="choice",
                    choices=['asciidoc','none'],
            help="Layout type. LAYOUT=<asciidoc>"),
            Option("--debug", action="store_true",
            dest="do_debug",
            help=SUPPRESS_HELP),
            Option("-v", "--verbose", action="store_true",
            dest="do_verbose", default=False,
            help="verbose output"),
        ]

        self.parser = OptionParser( usage=self.usage, 
                                    option_list=self.option_list)
        (self.options, self.args) = self.parser.parse_args()

        if len(self.args) != 1:
            self.parser.print_help()
            sys.exit(1)

        self.options.infile = self.args[0]


    def systemcmd(self, cmd):
        if self.options.do_verbose:
            msg = 'Execute: %s' % cmd
            sys.stderr.write(msg + os.linesep)
        else:
            cmd += ' 2>/dev/null'
        if os.system(cmd):
            raise EApp, 'failed command: %s' % cmd

    def run_for_real(self, infile, outfile):
        '''Convert Graphviz notation in file infile to PNG file named outfile.'''

        outfile = os.path.abspath(outfile)
        outdir = os.path.dirname(outfile)

        if not os.path.isdir(outdir):
            raise EApp, 'directory does not exist: %s' % outdir

        basefile = os.path.splitext(outfile)[0]
        saved_cwd = os.getcwd()
        os.chdir(outdir)


        try:
            #########################################################  lvv

            ####  check MPL version

            str_ver=matplotlib.__version__.split('.')
            ver=float(str_ver[0]) + float(str_ver[1])/1000
            if    ver < 0.098:   # extra 0 after dot - devider for minor ver is 1000
                sys.stderr.write('mplw warning: for mutplotlib version < 0.98 styles are disabled')
                self.options.style = 'none'

            ####  READ PY CODE

            eval_lines=''
            embeded_data = False

            for  line in infile:
                if   line.startswith('___'): 
                    embeded_data = True
                    break
                eval_lines += line


            ####  READ DATA

            if  embeded_data: 
                m = []  # matrix

                #  TODO replace csv with  http://matplotlib.sourceforge.net/api/mlab_api.html#matplotlib.mlab.csv2rec
                # aslo see http://matplotlib.sourceforge.net/api/mlab_api.html
                for row in csv.reader(infile, delimiter=',', quotechar="'", skipinitialspace=True):
                    if row:    # if not blank line
                        m.append(row)  

                    # convert to float if it look like number
                    for i in range(len(m[-1])):
                        if   len(m[-1][i].translate(string.maketrans('',''),' +-0123456789eE.')) == 0:
                            m[-1][i] = float(m[-1][i])

                c = [[row[i] for row in m] for i in range(len(m[0]))]   # transpose

            infile.close()


            ####  EVAL

            exec eval_lines


            if    self.options.style == 'asciidoc':

                auto_adjust(gcf())

                grid(True, color='0.7')
                ### TODO GRIDS
                #rcParams['grid.color'] = 'g'  # does not work
                #grid.color       :   black   # grid color
                #grid.linestyle   :   :       # dotted
                #grid.linewidth   :   0.5     # in points

                savefig(outfile, facecolor='0.95', edgecolor='0.8') # MPL bug? not all edges(borders) are drawn
                # TODO axes.linewidth      : 1.0     # edge linewidth
            else:
                savefig(outfile)

                if    self.options.style != 'none' :
                    sys.stderr.write('mplw warning: unknown style - ignored')
                   
            #########################################################

        finally:
            os.chdir(saved_cwd)

    def run(self):
        if self.options.infile == '-':
            sys.stdout.write(' ')       # To suppress asciidoc 'no output from filter' warnings.

            if self.options.outfile is None:
                sys.stderr.write('OUTFILE must be specified')
                sys.exit(1)
            infile = sys.stdin

        else:
            if not os.path.isfile(self.options.infile):
                raise EApp, 'input file does not exist: %s' % self.options.infile
            infile = open(self.options.infile)

        if self.options.outfile is None:
            outfile = os.path.splitext(self.options.infile)[0] + '.png'
        else:
            outfile = self.options.outfile

        self.run_for_real(infile, outfile)


def benchmark(label, val, label_part=-1):
    bar_width = 0.35
    ytick_pos = arange(len(val))+.5
    label.reverse()
    val.reverse()
    # 
    fontsize = rcParams['font.size']
    fixed_part = fontsize/72 * 3  

    h = (len(val)+1.4)*bar_width + fixed_part
    gcf().set_figheight(h)

    #step = ytick_pos[1] - ytick_pos[0]
    #gca().set_ylim(ytick_pos[0]-1, ytick_pos[-1]+1)

    yticks(ytick_pos, label, fontsize='large')
    barh(ytick_pos, val, align="center", height=0.6)
    gca().set_ybound(lower=ytick_pos[0]-0.7, upper=ytick_pos[-1]+0.7)
    gca().set_xbound(upper=max(val)*1.1)
    rcParams['axes.labelsize'] = 'large'

def auto_adjust(fig):
    axes =  fig.get_axes()

    h = fig.get_figheight()             # inch
    w = fig.get_figwidth()                  # inch
    fontsize = rcParams['font.size']    # point
    dpi = rcParams['savefig.dpi']       # point / inch

    # top,  title
    top_space = 1.7   # em

    if  axes[0].get_title():  # if there is a title  # FIXME: MPL bug? always true
        title_fontsize = matplotlib.font_manager.font_scalings[rcParams['axes.titlesize']] * fontsize
        top_adjust = 1.0 - title_fontsize/72 * top_space  /h 
        fig.subplots_adjust(top=top_adjust)

    # bottom,  xlabel
    bottom_space = 1.3  # em

    xtick_fontsize = matplotlib.font_manager.font_scalings[rcParams['xtick.labelsize']] * fontsize
    bottom_adjust = xtick_fontsize/72 /h * bottom_space
    if  len(axes[0].get_xlabel()) != 0:  #  xlabel
        xlabel_fontsize = matplotlib.font_manager.font_scalings[rcParams['axes.labelsize']] * fontsize
        bottom_adjust += xlabel_fontsize/72 /h

    fig.subplots_adjust(bottom=bottom_adjust)

    # left labels
    char_width = 0.8    # em

    current = gca().get_position().get_points()
    ll = gca().get_yticklabels()
    max_ytick_length = max([len(l.get_text()) for l in ll])
    max_ytick_length = max(6, max_ytick_length)
    ytick_fontsize = matplotlib.font_manager.font_scalings[rcParams['ytick.labelsize']] * fontsize
    left_adjust = max_ytick_length * char_width * ytick_fontsize/72 /w
    if  len(axes[0].get_ylabel()) > 0:   # ylable   # FIXME: MPL bug? always true
        ylabel_fontsize = matplotlib.font_manager.font_scalings[rcParams['axes.labelsize']] * fontsize
        left_adjust += ylabel_fontsize/72 /w
    fig.subplots_adjust(left=left_adjust)

    # righ margin
    right_margin = 1.5  # em
    #fig.subplots_adjust(right=1.0-fontsize/72/w * right_margin, hspace=0.2)


if __name__ == "__main__":
    app = Application()
    app.run()

# vim:ts=4 et sw=4 ft=python:
