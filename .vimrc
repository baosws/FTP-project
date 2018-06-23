syntax on

" set clipboard=unnamedplus
set shiftwidth=4
set expandtab
set nocompatible
set backspace=indent,eol,start
set ruler
set number
set smartindent
set showcmd
set tabstop=4
set splitbelow
set splitright
set laststatus=2
set statusline=%f
set autoread
set wrap
set autowriteall
set autoread
set pastetoggle=<F2>
set background=dark
set t_Co=256
set incsearch

set dir=/tmp

set mouse=a
filetype off

set rtp+=~/.vim/bundle/Vundle.vim

call vundle#begin()
	Plugin 'VundleVim/Vundle.vim'
	Plugin 'Valloric/YouCompleteMe'
	Plugin 'sCRooloose/syntastic'
	Plugin 'christoomey/vim-tmux-navigator'
	Plugin 'scrooloose/nerdtree'
call vundle#end()

set statusline+=%#wariningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*

let g:syntastic_cpp_compiler = 'g++'
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
let g:ycm_python_binary_path = '/usr/bin/python3'
let g:ycm_global_ycm_extra_conf = '~/.vim/ycm_extra_conf.py'
let g:ycm_confirm_extra_conf = 0
let g:ycm_filepath_completion_use_working_dir = 0
let g:ycm_rust_src_path = '~/.cargo/bin/rustc'
let g:ycm_autoclose_preview_window_after_completion = 1
let g:syntastic_python_checkers = ['python3']
let g:NERDTreeChDirMode = 2

let g:NERDTreeDirArrowExpandable = '+'
let g:NERDTreeDirArrowCollapsible = '-'
" let g:NERDTreeShowHidden = 1

filetype plugin indent on

function! SwapDown()
	if line(".") < line("$")
		exec ":m " line(".")+1
	endif
endfunction
function! SwapUp()
	if line(".") > 1
		exec ":m " line(".")-2
	endif
endfunction 

inoremap {<CR> {<CR>}<esc>O
let b:comment_leader='# '
autocmd Filetype java,cpp let b:comment_leader = '// '
autocmd Filetype vim,sh let b:comment_leader = '" '

noremap ,/ :<C-B><C-E>s/^/<C-R>=escape(b:comment_leader,'\/')<CR>/<CR>:noh<CR>
noremap ,\ :<C-B><C-E>s/^\V<C-R>=escape(b:comment_leader,'\/')<CR>//e<CR>:noh<CR>

noremap <C-up> :call SwapUp() <CR>
noremap <C-down> :call SwapDown() <CR>

inoremap kj ~<bs><esc>

nnoremap <C-p> :NERDTreeToggle<CR>

augroup myvimrchooks
    au!
    autocmd bufwritepost .vimrc source ~/.vimrc
augroup END

au CursorHold,CursorHoldI * checktime

vnoremap <C-c> "+y
vnoremap <C-v> "-p
nnoremap <C-a> gg0vG$

augroup templates
	autocmd BufNewFile *.java 0r ~/Templates/java.template
	autocmd BufNewFile *.cpp 0r ~/Templates/cpp.template
augroup END

noremap j gj
noremap k gk
noremap J j
noremap K k
vnoremap j gj
vnoremap k gk
vnoremap J j
vnoremap K k

noremap <up> gk
noremap <down> gj
vnoremap <up> gk
vnoremap <down> gj

nnoremap o $a<CR>
noremap L g$
noremap H g^
vnoremap L g$
vnoremap H g^

nnoremap ss :w<CR>
nnoremap s :w<CR>
nnoremap <C-d> :q<CR>
nnoremap rr :source ~/.vimrc<CR>:e!<CR>
