CREATE DATABASE QuanLyThuVien
GO

USE QuanLyThuVien
GO

CREATE TABLE CuonSach (
	isbn int,
	Ma_CuonSach smallint,
	TinhTrang nvarchar(1),
	CONSTRAINT PK_cuonsach PRIMARY KEY (isbn,Ma_CuonSach)
)
GO

CREATE TABLE DangKy (
	isbn int,
	ma_docgia smallint,
	ngaygio_dk smalldatetime,
	ghichu nvarchar(255),
	CONSTRAINT PK_dangky PRIMARY KEY (isbn, ma_docgia)
)
GO

CREATE TABLE DauSach (
	isbn int,
	ma_tuasach int,
	ngonngu nvarchar(15),
	bia nvarchar(15),
	trangthai nvarchar(1),
	CONSTRAINT PK_dausach PRIMARY KEY (isbn)
)
GO

CREATE TABLE DocGia (
	ma_docgia smallint,
	ho nvarchar(15),
	tenlot nvarchar(1),
	ten nvarchar(15),
	NgaySinh smalldatetime,
	CONSTRAINT PK_docgia PRIMARY KEY (ma_docgia)
)
GO

CREATE TABLE Muon (
	isbn int,
	ma_cuonsach smallint,
	ma_docgia smallint,
	ngayGio_muon smalldatetime,
	ngay_hethan smalldatetime,
	CONSTRAINT PK_muon PRIMARY KEY (isbn, ma_cuonsach)
)
GO

CREATE TABLE NguoiLon (
	ma_docgia smallint,
	sonha nvarchar(15),
	duong nvarchar(63),
	quan nvarchar(2),
	dienthoai nvarchar(13),
	han_sd smalldatetime,
	CONSTRAINT PK_nguoilon PRIMARY KEY (ma_docgia)
)
GO

CREATE TABLE QuaTrinhMuon (
	isbn int,
	ma_cuonsach smallint,
	ngayGio_muon smalldatetime,
	ma_docgia smallint,
	ngay_hethan smalldatetime,
	ngayGio_tra smalldatetime,
	tien_muon money,
	tien_datra money,
	tien_datcoc money,
	ghichu nvarchar(255),
	CONSTRAINT PK_QuaTrinhMuon PRIMARY KEY (isbn, ma_cuonsach, ngayGio_muon)
)
GO

CREATE TABLE TreEm (
	ma_docgia smallint,
	ma_docgia_nguoilon smallint,
	CONSTRAINT PK_treem PRIMARY KEY (ma_docgia)
)
GO

CREATE TABLE TuaSach(
	ma_tuasach int,
	TuaSach nvarchar(63),
	tacgia nvarchar(31),
	tomtat varchar(222),
	CONSTRAINT PK_tuasach PRIMARY KEY (ma_tuasach)
)
GO

------------------

ALTER TABLE CuonSach WITH NOCHECK 
ADD CONSTRAINT FK_cuonsach_dausach FOREIGN KEY(isbn)
REFERENCES DauSach (isbn)
GO

ALTER TABLE CuonSach CHECK CONSTRAINT FK_cuonsach_dausach
GO

ALTER TABLE DangKy WITH NOCHECK 
ADD CONSTRAINT FK_dangky_dausach FOREIGN KEY(isbn)
REFERENCES DauSach (isbn)
GO

ALTER TABLE DangKy CHECK CONSTRAINT FK_dangky_dausach
GO

ALTER TABLE DangKy WITH NOCHECK
ADD CONSTRAINT FK_dangky_docgia FOREIGN KEY(ma_docgia)
REFERENCES DocGia (ma_docgia)
GO

ALTER TABLE DangKy CHECK CONSTRAINT FK_dangky_docgia
GO

ALTER TABLE DauSach WITH NOCHECK 
ADD CONSTRAINT FK_dausach_tuasach FOREIGN KEY(ma_tuasach)
REFERENCES TuaSach (ma_tuasach)
GO

ALTER TABLE DauSach CHECK CONSTRAINT FK_dausach_tuasach
GO

ALTER TABLE Muon WITH NOCHECK 
ADD CONSTRAINT FK_muon_cuonsach FOREIGN KEY(isbn, ma_cuonsach)
REFERENCES CuonSach (isbn, Ma_CuonSach)
GO

ALTER TABLE Muon CHECK CONSTRAINT FK_muon_cuonsach
GO

ALTER TABLE Muon WITH NOCHECK 
ADD CONSTRAINT fk_Muon_DocGia FOREIGN KEY(ma_docgia)
REFERENCES DocGia (ma_docgia)
GO

ALTER TABLE Muon CHECK CONSTRAINT fk_Muon_DocGia
GO

ALTER TABLE NguoiLon WITH NOCHECK 
ADD CONSTRAINT FK_nguoilon_docgia FOREIGN KEY(ma_docgia)
REFERENCES DocGia (ma_docgia)
GO

ALTER TABLE NguoiLon CHECK CONSTRAINT FK_nguoilon_docgia
GO

ALTER TABLE QuaTrinhMuon WITH NOCHECK
ADD CONSTRAINT FK_QuaTrinhMuon_cuonsach FOREIGN KEY(isbn, ma_cuonsach)
REFERENCES CuonSach (isbn, Ma_CuonSach)
GO

ALTER TABLE QuaTrinhMuon CHECK CONSTRAINT FK_QuaTrinhMuon_cuonsach
GO

ALTER TABLE QuaTrinhMuon WITH NOCHECK
ADD CONSTRAINT fk_QuaTrinhMuon_DocGia FOREIGN KEY(ma_docgia)
REFERENCES DocGia (ma_docgia)
GO

ALTER TABLE QuaTrinhMuon CHECK CONSTRAINT fk_QuaTrinhMuon_DocGia
GO

ALTER TABLE TreEm WITH NOCHECK 
ADD CONSTRAINT FK_treem_docgia FOREIGN KEY(ma_docgia)
REFERENCES DocGia (ma_docgia)
GO

ALTER TABLE TreEm CHECK CONSTRAINT FK_treem_docgia
GO

ALTER TABLE TreEm WITH NOCHECK
ADD CONSTRAINT FK_treem_nguoilon FOREIGN KEY(ma_docgia_nguoilon)
REFERENCES NguoiLon (ma_docgia)
GO

ALTER TABLE TreEm CHECK CONSTRAINT FK_treem_nguoilon
GO

--------------------

--DocGia
insert into DocGia values ('1', 'Pham', 'V', 'Bao', null)
insert into DocGia values ('2', 'Vo', 'V', 'Dung', null)
insert into DocGia values ('3', 'Van', 'P', 'Phuc', null)
insert into DocGia values ('4', 'Bui', 'N', 'Binh', null)
insert into DocGia values ('5', 'Le', 'V', 'Sang', null)
insert into DocGia values ('6', 'Vo', 'H', 'Ha', null)
insert into DocGia values ('7', 'Le', 'B', 'Mong', null)
insert into DocGia values ('8', 'Vu', 'C', 'Huong', null)
insert into DocGia values ('9', 'Dinh', 'B', 'Ha', null)
insert into DocGia values ('10', 'Ho', 'V', 'My', null)
insert into DocGia values ('11', 'Dinh', 'L', 'Mong', null)
insert into DocGia values ('12', 'Do', 'B', 'Phuong', null)
insert into DocGia values ('13', 'Dong', 'L', 'Binh', null)
insert into DocGia values ('14', 'Than', 'P', 'My', null)
insert into DocGia values ('15', 'Phan', 'N', 'My', null)

--NguoiLon
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('1', '10', 'Bui Thi Xuan', '2', '6/20/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('3', '20', 'Dinh Bo Linh', '4', '6/22/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('5', '30', 'Duong Ba Trac', '6', '6/24/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('7', '40', 'Huynh Khuong Ninh', '8', '6/26/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('9', '50', 'Le Loi', '10', '6/28/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('11', '60', 'Le Thi Rieng', 'BT', '6/30/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('13', '70', 'Nguyen Trai', 'GV', '7/2/2002')
insert into NguoiLon (ma_docGia, soNha, duong, quan, han_sd) values ('15', '80', 'Pham Ngu Lao', 'TD', '7/4/2002')

--TreEm
insert into TreEm values ('2', '1')
insert into TreEm values ('4', '3')
insert into TreEm values ('6', '5')
insert into TreEm values ('8', '7')
insert into TreEm values ('10', '9')
insert into TreEm values ('12', '11')
insert into TreEm values ('14', '13')

--TuaSach
insert into TuaSach values (1, 'Last of the Mohicans', 'James Fenimore Cooper', null)
insert into TuaSach values (2, 'The Night-Born', 'Jack London', null)
insert into TuaSach values (3, 'Lemon', 'Motojirou', null)
insert into TuaSach values (4, 'Walking', 'Henry David Thoreau', null)
insert into TuaSach values (5, 'An may di vang', 'Chu Lai', null)
insert into TuaSach values (6, 'Dat rung phuong nam', 'Doan Gioi', null)
insert into TuaSach values (7, 'Noi bat hanh tinh yeu', 'Hoang Lai Giang', null)
insert into TuaSach values (8, 'Con giong', 'Le Van Thao', null)
insert into TuaSach values (9, 'Den vo, do long', 'Mai Thanh Hai', null)
insert into TuaSach values (10, 'Thuyen ve ben Ngu', 'Mong Binh Son', null)
insert into TuaSach values (11, 'Cong tu Bac Lieu', 'Nguyen Hung', null)
insert into TuaSach values (12, 'The Village Watch-Tower', 'Kate Douglas Wiggin', null)
insert into TuaSach values (13, 'Tra gia', 'Trieu Xuan', null)
insert into TuaSach values (14, 'Nu kiet Tong Khanh Linh', 'Tuong Hong Ban', null)
insert into TuaSach values (15, 'Trang suc nguoi Viet co', 'Trinh Sinh', null)

--DauSach
insert into DauSach values (1, 1, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (3, 1, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (4, 1, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (5, 1, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (6, 1, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (7, 1, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (9, 1, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (10, 1, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (11, 2, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (13, 2, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (14, 2, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (15, 2, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (16, 2, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (17, 2, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (19, 2, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (20, 2, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (21, 3, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (23, 3, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (24, 3, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (25, 3, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (26, 3, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (27, 3, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (29, 3, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (30, 3, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (31, 4, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (33, 4, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (34, 4, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (35, 4, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (36, 4, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (37, 4, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (39, 4, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (40, 4, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (41, 5, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (43, 5, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (44, 5, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (45, 5, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (46, 5, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (47, 5, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (49, 5, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (50, 5, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (51, 6, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (53, 6, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (54, 6, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (55, 6, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (56, 6, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (57, 6, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (59, 6, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (60, 6, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (61, 7, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (63, 7, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (64, 7, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (65, 7, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (66, 7, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (67, 7, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (69, 7, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (70, 7, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (71, 8, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (73, 8, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (74, 8, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (75, 8, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (76, 8, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (77, 8, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (79, 8, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (80, 8, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (81, 9, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (83, 9, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (84, 9, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (85, 9, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (86, 9, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (87, 9, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (89, 9, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (90, 9, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (91, 10, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (93, 10, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (94, 10, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (95, 10, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (96, 10, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (97, 10, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (99, 10, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (100, 10, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (101, 11, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (103, 11, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (104, 11, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (105, 11, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (106, 11, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (107, 11, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (109, 11, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (110, 11, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (111, 12, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (113, 12, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (114, 12, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (115, 12, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (116, 12, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (117, 12, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (119, 12, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (120, 12, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (121, 13, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (123, 13, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (124, 13, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (125, 13, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (126, 13, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (127, 13, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (129, 13, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (130, 13, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (131, 14, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (133, 14, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (134, 14, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (135, 14, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (136, 14, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (137, 14, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (139, 14, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (140, 14, 'Y', 'BIA CUNG', 'Y')
insert into DauSach values (141, 15, 'ANH', 'BIA CUNG', 'Y')
insert into DauSach values (143, 15, 'DUC', 'BIA CUNG', 'Y')
insert into DauSach values (144, 15, 'HAN', 'BIA CUNG', 'Y')
insert into DauSach values (145, 15, 'HOA', 'BIA CUNG', 'Y')
insert into DauSach values (146, 15, 'NHAT', 'BIA CUNG', 'Y')
insert into DauSach values (147, 15, 'PHAP', 'BIA CUNG', 'Y')
insert into DauSach values (149, 15, 'VIET', 'BIA CUNG', 'Y')
insert into DauSach values (150, 15, 'Y', 'BIA CUNG', 'Y')

--CuonSach
insert into CuonSach values (1,1, 'N')
insert into CuonSach values (1,2, 'N')
insert into CuonSach values (1,3, 'Y')
insert into CuonSach values (1,4, 'N')
insert into CuonSach values (1,5, 'Y')
insert into CuonSach values (3,1, 'Y')
insert into CuonSach values (3,2, 'N')
insert into CuonSach values (3,3, 'N')
insert into CuonSach values (3,4, 'N')
insert into CuonSach values (3,5, 'N')
insert into CuonSach values (4,1, 'N')
insert into CuonSach values (4,2, 'N')
insert into CuonSach values (4,3, 'N')
insert into CuonSach values (4,4, 'N')
insert into CuonSach values (4,5, 'Y')
insert into CuonSach values (5,1, 'N')
insert into CuonSach values (5,2, 'N')
insert into CuonSach values (5,3, 'N')
insert into CuonSach values (5,4, 'Y')
insert into CuonSach values (5,5, 'N')
insert into CuonSach values (6,1, 'N')
insert into CuonSach values (6,2, 'N')
insert into CuonSach values (6,3, 'Y')
insert into CuonSach values (6,4, 'N')
insert into CuonSach values (6,5, 'N')
insert into CuonSach values (7,1, 'N')
insert into CuonSach values (7,2, 'Y')
insert into CuonSach values (7,3, 'N')
insert into CuonSach values (7,4, 'N')
insert into CuonSach values (7,5, 'N')
insert into CuonSach values (9,1, 'N')
insert into CuonSach values (9,2, 'N')
insert into CuonSach values (9,3, 'N')
insert into CuonSach values (9,4, 'N')
insert into CuonSach values (9,5, 'Y')
insert into CuonSach values (10,1, 'N')
insert into CuonSach values (10,2, 'N')
insert into CuonSach values (10,3, 'N')
insert into CuonSach values (10,4, 'Y')
insert into CuonSach values (10,5, 'N')
insert into CuonSach values (11,1, 'N')
insert into CuonSach values (11,2, 'N')
insert into CuonSach values (11,3, 'Y')
insert into CuonSach values (11,4, 'N')
insert into CuonSach values (11,5, 'N')
insert into CuonSach values (13,1, 'Y')
insert into CuonSach values (13,2, 'N')
insert into CuonSach values (13,3, 'N')
insert into CuonSach values (13,4, 'N')
insert into CuonSach values (13,5, 'N')
insert into CuonSach values (14,1, 'N')
insert into CuonSach values (14,2, 'N')
insert into CuonSach values (14,3, 'N')
insert into CuonSach values (14,4, 'N')
insert into CuonSach values (14,5, 'Y')
insert into CuonSach values (15,1, 'N')
insert into CuonSach values (15,2, 'N')
insert into CuonSach values (15,3, 'N')
insert into CuonSach values (15,4, 'Y')
insert into CuonSach values (15,5, 'N')

--DangKy
insert into DangKy values (1,1, '6/19/2002', null)

--Muon
insert into Muon values (1, 3, 2, '6/6/2002', '6/20/2002')
insert into Muon values (1, 5, 1, '6/19/2002', '7/3/2002')
insert into Muon values (3, 1, 1, '6/6/2002', '6/20/2002')
insert into Muon values (4, 5, 1, '6/7/2002', '6/21/2002')
insert into Muon values (5, 4, 1, '6/7/2002', '6/21/2002')
insert into Muon values (6, 3, 1, '6/5/2002', '6/19/2002')
insert into Muon values (7, 2, 1, '6/17/2002', '7/1/2002')
insert into Muon values (9, 5, 2, '6/16/2002', '6/30/2002')
insert into Muon values (10, 4, 2, '6/11/2002', '6/25/2002')
insert into Muon values (11, 3, 2, '6/4/2002', '6/18/2002')
insert into Muon values (13, 1, 2, '6/16/2002', '6/30/2002')
insert into Muon values (14, 5, 2, '6/9/2002', '6/23/2002')
insert into Muon values (15, 4, 2, '6/15/2002', '6/29/2002')

--QTrinhMuon

insert into QuaTrinhMuon values (14, 5, '6/24/2001', 2, '7/8/2001', '6/30/2001', null, null, null, null)
insert into QuaTrinhMuon values (14, 5, '7/8/2001', 2, '7/22/2001', '7/14/2001', null, null, null, null)
insert into QuaTrinhMuon values (14, 5, '7/22/2001', 2, '8/5/2001', '7/28/2001', null, null, null, null)