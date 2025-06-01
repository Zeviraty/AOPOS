;===========================================================================
; src/drivers/ata_rw.asm  — 32-bit Protected Mode (cdecl) version
;
; void ata_lba_read(uint32_t lba, uint8_t count, void* buffer);
; void ata_lba_write(uint32_t lba, uint8_t count, void* buffer);
;
; In cdecl, arguments land on the stack as:
;   [ebp+ 8] = lba       (uint32_t)
;   [ebp+12] = count     (uint8_t, but it's 32-bit slot)
;   [ebp+16] = buffer    (void*)
;===========================================================================

[BITS 32]
global ata_lba_read
global ata_lba_write

;-----------------------------------------
; ata_lba_read: read 'count' sectors at 'lba' into [buffer]
;-----------------------------------------
ata_lba_read:
    push    ebp
    mov     ebp, esp

    ; pull args into registers before clobbering general regs:
    mov     eax, [ebp + 8]    ; eax = lba
    mov     ecx, [ebp + 12]   ; ecx = count (only low byte, but ok)
    mov     edi, [ebp + 16]   ; edi = buffer pointer

    pushad                   ; save all general-purpose regs
    pushfd                   ; save EFLAGS

    ;—► DRIVE/HEAD + LBA[24:27]
    mov     ebx, eax         ; ebx = full lba
    mov     dx, 0x1F6
    mov     al, 0xE0         ; 1110 0000b = LBA mode + master
    shr     ebx, 24          ; now ebx's low 8 bits = LBA[24:31]
    and     bl, 0x0F         ; we only want bits 24–27
    or      al, bl           
    out     dx, al

    ;—► SECTOR COUNT
    mov     dx, 0x1F2
    mov     al, cl           ; cl = count
    out     dx, al

    ;—► LBA[0:7]
    mov     dx, 0x1F3
    mov     al, al           ; low 8 bits of eax (lba[0:7])
    out     dx, al

    ;—► LBA[8:15]
    mov     dx, 0x1F4
    mov     al, ah           ; ah = (lba >> 8) & 0xFF
    out     dx, al

    ;—► LBA[16:23]
    shr     eax, 16          ; now AL = (lba >> 16) & 0xFF
    mov     dx, 0x1F5
    out     dx, al

    ;—► SEND READ COMMAND (0x20)
    mov     dx, 0x1F7
    mov     al, 0x20         ; "Read sectors with retry"
    out     dx, al

    ;—► WAIT until BSY=0, then DRQ=1
.wait_bsy:
    in      al, dx
    test    al, 0x80         ; BSY (bit 7)
    jnz     .wait_bsy

.wait_drq:
    in      al, dx
    test    al, 0x08         ; DRQ (bit 3)
    jz      .wait_drq

    ;—► READ data: 256 words per sector; total words = 256 * count
    mov     eax, [ebp + 8]   ; reload lba into eax (we shifted it earlier)
    mov     ecx, [ebp + 12]  ; reload count
    mov     ebx, 256
    mul     ebx              ; eax = 256 * count  (words to read)
    mov     ecx, eax         ; ecx = total words

    mov     dx, 0x1F0        ; data port
    cld                      ; forward direction
    rep     insw             ; read CX words from DX → [EDI]

    popfd
    popad

    mov     esp, ebp
    pop     ebp
    ret


;-----------------------------------------
; ata_lba_write: write 'count' sectors at 'lba' from [buffer]
;-----------------------------------------
ata_lba_write:
    push    ebp
    mov     ebp, esp

    ; pull args into registers first:
    mov     eax, [ebp + 8]    ; eax = lba
    mov     ecx, [ebp + 12]   ; ecx = count
    mov     esi, [ebp + 16]   ; esi = buffer pointer

    pushad                    ; save all general-purpose regs
    pushfd                    ; save EFLAGS

    ;—► DRIVE/HEAD + LBA[24:27]
    mov     ebx, eax          ; ebx = full lba
    mov     dx, 0x1F6
    mov     al, 0xE0          ; master + LBA mode
    shr     ebx, 24           ; ebx’s low byte = LBA[24:31]
    and     bl, 0x0F
    or      al, bl
    out     dx, al

    ;—► SECTOR COUNT
    mov     dx, 0x1F2
    mov     al, cl            ; count
    out     dx, al

    ;—► LBA[0:7]
    mov     dx, 0x1F3
    mov     al, al            ; low 8 bits of eax
    out     dx, al

    ;—► LBA[8:15]
    mov     dx, 0x1F4
    mov     al, ah            ; (lba >> 8) & 0xFF
    out     dx, al

    ;—► LBA[16:23]
    shr     eax, 16           ; now AL = (lba >> 16) & 0xFF
    mov     dx, 0x1F5
    out     dx, al

    ;—► SEND WRITE COMMAND (0x30)
    mov     dx, 0x1F7
    mov     al, 0x30          ; "Write sectors with retry"
    out     dx, al

    ;—► WAIT until BSY=0, then DRQ=1
.wait_bsy_w:
    in      al, dx
    test    al, 0x80          ; BSY bit
    jnz     .wait_bsy_w

.wait_drq_w:
    in      al, dx
    test    al, 0x08          ; DRQ bit
    jz      .wait_drq_w

    ;—► WRITE data: 256 words per sector; total words = 256 * count
    mov     eax, [ebp + 8]    ; reload lba into eax
    mov     ecx, [ebp + 12]   ; reload count
    mov     ebx, 256
    mul     ebx               ; eax = 256 * count
    mov     ecx, eax          ; ecx = total words

    mov     dx, 0x1F0         ; data port
    cld                       ; forward direction
    rep     outsw             ; write CX words from [ESI] → DX

    popfd
    popad

    mov     esp, ebp
    pop     ebp
    ret

