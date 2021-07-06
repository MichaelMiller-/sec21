import { Entity, PrimaryGeneratedColumn, Column, OneToOne, JoinColumn } from "typeorm";
import { CurveMember } from "./CurveMember";

//! \todo naming
@Entity()
export class CurveMemberResult {

   @PrimaryGeneratedColumn()
   id: number;

   @Column("real")
   N: number = 0; // [N]

   @OneToOne(type => CurveMember)
   @JoinColumn()
   member: CurveMember;
}
