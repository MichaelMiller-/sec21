import { useEffect, useState } from "react";
import { Spinner, Table, Alert } from "react-bootstrap";
import { DbCurveMember } from "./DatabaseInterface";
import CopyButton from "./ui/CopyButton";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import axios from 'axios';

const CurveMemberListItem = (props: DbCurveMember) => {
   const onEdit = () => { }
   const onCopy = () => { }
   const onDelete = async () => {
      const res = await axios.delete(process.env.REACT_APP_BACKEND + '/curvemember/' + props.id)
      if (res.data.success === false) {
         //! \todo handle error
      }
      window.location.reload();
   }
   return (
      <tr>
         <td><EditButton onClick={onEdit} disabled={true}></EditButton></td>
         <td>{props.name}</td>
         <td>{props.beginNode.name}</td>
         <td>{props.endNode.name}</td>
         <td>{props.systemLine}</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={false} /></td>
      </tr>
   )
}

export type CurveMemberListProps = {
   projectId: string;
}

const CurveMemberList = (props: CurveMemberListProps) => {

   const [data, setData] = useState<DbCurveMember[]>([])
   const [loading, setLoading] = useState(true)

   const getData = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/curvemembers/' + props.projectId)
         .then(response => setData(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => {
      getData()
      // eslint-disable-next-line react-hooks/exhaustive-deps
   }, []);

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   if (data.length === 0) {
      return (<Alert variant='danger'>No members defined.</Alert>);
   }

   return (
      <>
         <Table borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>Begin node</th>
                  <th>End node</th>
                  <th>Systemline</th>
               </tr>
            </thead>
            <tbody>
               {data.map(e => (
                  <CurveMemberListItem
                     id={e.id}
                     name={e.name}
                     uuid={e.uuid}
                     beginNode={e.beginNode}
                     endNode={e.endNode}
                     systemLine={e.systemLine}
                  />))}
            </tbody>
         </Table>
      </>
   )
}

export default CurveMemberList;